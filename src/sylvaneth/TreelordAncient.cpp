/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Dice.h>
#include <sylvaneth/TreelordAncient.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include <sylvaneth/SylvanethSpells.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {

    class HeedTheSpiritsong : public CommandAbility {
    public:
        explicit HeedTheSpiritsong(Unit *source);

    protected:
        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }
    };

    HeedTheSpiritsong::HeedTheSpiritsong(Unit *source) :
            CommandAbility(source, "Heed the Spirit-song", 12, 12, Phase::Hero) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords = {SYLVANETH};
    }

    bool HeedTheSpiritsong::apply(Unit *target) {

        auto units = Board::Instance()->getUnitsWithin(m_source->position(), m_source->owningPlayer(), m_rangeGeneral);
        for (auto unit : units) {
            if (unit->hasKeyword(SYLVANETH)) {
                unit->buffReroll(Attribute::To_Save_Missile, Rerolls::Ones,
                                 {m_phase, m_round + 1, m_source->owningPlayer()});
                unit->buffReroll(Attribute::To_Save_Melee, Rerolls::Ones,
                                 {m_phase, m_round + 1, m_source->owningPlayer()});
            }
        }
        return true;
    }

    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 260;

    bool TreelordAncient::s_registered = false;

    struct TableEntry {
        int m_staffToHit;
        int m_sweepingBlowsAttacks;
        int m_talonToWound;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {2, 3, 2},
                    {3, 2, 2},
                    {4, 2, 3},
                    {5, 1, 3},
                    {6, 1, 4}
            };

    TreelordAncient::TreelordAncient() :
            SylvanethBase("Treelord Ancient", 5, g_wounds, 9, 3, false),
            m_doomTendrilStaff(Weapon::Type::Missile, "Doom Tendril Staff", 18, 1, 2, 3, -1, RAND_D6),
            m_sweepingBlows(Weapon::Type::Melee, "Sweeping Blows", 3, 3, 3, 3, -1, RAND_D6),
            m_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1) {
        m_keywords = {ORDER, SYLVANETH, NOBLE_SPIRITS, MONSTER, HERO, WIZARD, TREELORD_ANCIENT};
        m_weapons = {&m_doomTendrilStaff, &m_sweepingBlows, &m_massiveImpalingTalons};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    void TreelordAncient::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_doomTendrilStaff);
        model->addMeleeWeapon(&m_sweepingBlows);
        model->addMeleeWeapon(&m_massiveImpalingTalons);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<HeedTheSpiritsong>(this));

        m_points = g_pointsPerUnit;
    }

    void TreelordAncient::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_doomTendrilStaff.setToHit(g_damageTable[damageIndex].m_staffToHit);
        m_sweepingBlows.setAttacks(g_damageTable[damageIndex].m_sweepingBlowsAttacks);
        m_massiveImpalingTalons.setToWound(g_damageTable[damageIndex].m_talonToWound);
    }

    size_t TreelordAncient::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();

        // Our Roots Run Deep
        if (hasKeyword(OAKENBROW)) woundsInflicted += 2;

        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *TreelordAncient::Create(const ParameterList &parameters) {
        auto unit = new TreelordAncient();

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfTheDeepwood[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_aspectsOfRenewal[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_relicsOfNature[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure(lore);
        return unit;
    }

    void TreelordAncient::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TreelordAncient::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    TreelordAncient::ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            EnumParameter("Lore", g_loreOfTheDeepwood[0], g_loreOfTheDeepwood),
                            EnumParameter("Command Trait", g_aspectsOfRenewal[0], g_aspectsOfRenewal),
                            EnumParameter("Artefact", g_relicsOfNature[0], g_relicsOfNature),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Treelord Ancient", factoryMethod);
        }
    }

    Wounds TreelordAncient::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Impale
        if ((hitRoll == 6) && (weapon->name() == m_massiveImpalingTalons.name())) {
            return {0, Dice::RollD6()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int TreelordAncient::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth
