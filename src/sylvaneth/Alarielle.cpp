/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Dice.h>
#include <sylvaneth/Alarielle.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <sylvaneth/SylvanethSpells.h>
#include <Board.h>
#include <Roster.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {

    class GhyransWrath : public CommandAbility {
    public:
        explicit GhyransWrath(Unit *source);

    protected:
        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }
    };

    GhyransWrath::GhyransWrath(Unit *source) :
            CommandAbility(source, "Ghyran's Wrath", 14, 14, GamePhase::Combat) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords = {SYLVANETH};
    }

    bool GhyransWrath::apply(Unit *target) {

        auto units = Board::Instance()->getUnitsWithin(m_source->position(), m_source->owningPlayer(), m_rangeGeneral);
        for (auto unit : units) {
            if (unit->hasKeyword(SYLVANETH)) {
                unit->buffReroll(Attribute::To_Wound_Melee, Rerolls::Ones, defaultDuration());
            }
        }
        return true;
    }

    static const int g_basesize = 160;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 740;

    bool Alarielle::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_spearKurnothDamage;
        int m_greatAntlerDamage;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 8, 10, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 6, 5},
                    {15, 5, 4},
                    {14, 4, 3},
                    {13,  3, 2},
                    {12,  2, 1}
            };


    Alarielle::Alarielle(Glade glade, bool isGeneral) :
            SylvanethBase("Alarielle", 16, g_wounds, 10, 3, true, g_pointsPerUnit) {
        m_keywords = {ORDER, SYLVANETH, MONSTER, HERO, WIZARD, ALARIELLE_THE_EVERQUEEN};
        m_weapons = {&m_spearOfKurnoth, &m_talonOfDwindling, &m_beetleGreatAntlers};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_beetleGreatAntlers.setMount(true);
        m_totalUnbinds = 3;
        m_totalSpells = 3;

        // Swirling Glowspites
        m_retreatAndCharge = true;
        m_retreatAndShoot = true;

        setGlade(glade);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_spearOfKurnoth);
        model->addMeleeWeapon(&m_talonOfDwindling);
        model->addMeleeWeapon(&m_beetleGreatAntlers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateMetamorphosis(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Throne_Of_Vines, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Regrowth, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::The_Dwellers_Below, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Deadly_Harvest, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Verdurous_Harmony, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Treesong, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<GhyransWrath>(this));

        m_points = g_pointsPerUnit;
    }

    void Alarielle::onStartHero(PlayerId player) {
        SylvanethBase::onStartHero(player);
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                // Lifebloom - heal herself 2D6
                int woundsHealed = Dice::Roll2D6();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }

            // Lifebloom
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 30.0);
            for (auto ip : units) {
                if (ip->hasKeyword(SYLVANETH))
                    ip->heal(Dice::RollD3());
            }
        }
    }

    void Alarielle::onWounded() {
        SylvanethBase::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_spearOfKurnoth.setDamage(g_damageTable[damageIndex].m_spearKurnothDamage);
        m_beetleGreatAntlers.setDamage(g_damageTable[damageIndex].m_greatAntlerDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t Alarielle::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *Alarielle::Create(const ParameterList &parameters) {
        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Alarielle(glade, general);
    }

    void Alarielle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Alarielle", factoryMethod);
        }
    }

    void Alarielle::onCharged() {
        // Living Battering Ram
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            auto roll = Dice::RollD6();
            if (roll >= 4) {
                Wounds wounds = {0, 0};
                wounds.mortal = Dice::RollD3();
                if (roll == 6)
                    wounds.mortal = Dice::RollD6();
                ip->applyDamage(wounds, this);
            }
        }
        SylvanethBase::onCharged();
    }

    void Alarielle::onEndMovement(PlayerId player) {
        SylvanethBase::onEndMovement(player);

        if (owningPlayer() == player && !m_usedSoulAmphorae) {
            // Summon a unit and add to roster.
            // TODO: select unit randomly, 20 Dryads, 10 Tree-Revenants, 10 Spite-Revenants, 3 Kurnoth Hunters, Branchwych or Treelord
            auto factory = UnitFactory::LookupUnit("Kurnoth Hunters");
            if (factory) {
                if (m_roster) {
                    auto unit = std::shared_ptr<Unit>(UnitFactory::Create("Kurnoth Hunters", factory->m_parameters));
                    unit->deploy(position(), orientation());
                    m_roster->addUnit(unit);
                }
                m_usedSoulAmphorae = true;
            }
        }
    }

    void Alarielle::onRestore() {
        SylvanethBase::onRestore();

        m_usedSoulAmphorae = false;

        // Restore table-driven attributes
        onWounded();
    }

    int Alarielle::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth
