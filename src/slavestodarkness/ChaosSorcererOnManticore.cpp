/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "slavestodarkness/ChaosSorcererOnManticore.h"
#include "SlavesToDarknessPrivate.h"
#include "StDSpells.h"

namespace SlavesToDarkness {

    class WindOfChaos : public Spell {
    public:
        explicit WindOfChaos(Unit *caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override;

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    WindOfChaos::WindOfChaos(Unit *caster) :
            Spell(caster, "Wind of Chaos", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result
    WindOfChaos::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        Dice::RollResult roll;
        Dice::RollD6(target->remainingModels(), roll);

        int totalWounds = roll.m_distribution[5];

        for (auto i = 0; i < roll.m_distribution[6]; i++) {
            totalWounds += Dice::RollD3();
        }
        target->applyDamage({0, totalWounds, Wounds::Source::Spell}, m_caster);

        return Spell::Result::Success;
    }

    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 260;

    struct TableEntry {
        int m_move;
        int m_fangsToWound;
        int m_tailAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 1, 5},
                    {10, 2, 4},
                    {8,  3, 3},
                    {6,  4, 2},
                    {6,  5, 1}
            };

    bool ChaosSorcererOnManticore::s_registered = false;

    Unit *ChaosSorcererOnManticore::Create(const ParameterList &parameters) {
        auto unit = new ChaosSorcererOnManticore();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosSorcererOnManticore::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosSorcererOnManticore::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosSorcererOnManticore::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Sorcerer On Manticore", factoryMethod);
        }
    }

    ChaosSorcererOnManticore::ChaosSorcererOnManticore() :
            SlavesToDarknessBase("Chaos Sorcerer On Manticore", 12, g_wounds, 8, 4, true),
            m_staff(Weapon::Type::Melee, "Sorcerous Reaping Staff", 2, 3, 3, 3, -1, RAND_D3),
            m_fangsAndClaws(Weapon::Type::Melee, "Honed Fangs and Claws", 1, 5, 3, 1, -1, 2),
            m_tail(Weapon::Type::Melee, "Shredding Tail", 3, 5, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, MANTICORE, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, MONSTER, HERO,
                      WIZARD, CHAOS_SORCERER_LORD};
        m_weapons = {&m_staff, &m_fangsAndClaws, &m_tail};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_fangsAndClaws.setMount(true);
        m_tail.setMount(true);
        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool ChaosSorcererOnManticore::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_fangsAndClaws);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<WindOfChaos>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    void ChaosSorcererOnManticore::onRestore() {
        SlavesToDarknessBase::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    void ChaosSorcererOnManticore::onWounded() {
        SlavesToDarknessBase::onWounded();
        const int damageIndex = getDamageTableIndex();
        m_fangsAndClaws.setToWound(g_damageTable[damageIndex].m_fangsToWound);
        m_tail.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t ChaosSorcererOnManticore::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls ChaosSorcererOnManticore::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Territorial Predator
        if ((weapon->name() == m_fangsAndClaws.name()) && (target->hasKeyword(MONSTER))) {
            return Rerolls::Failed;
        }
        return SlavesToDarknessBase::toHitRerolls(weapon, target);
    }

    int ChaosSorcererOnManticore::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace SlavesToDarkness
