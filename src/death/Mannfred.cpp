/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Mannfred.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 340;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
        double m_vigourRange;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 6, 15},
                    {13, 5, 12},
                    {10, 4, 9},
                    {7,  3, 6},
                    {4,  2, 3}
            };

    bool MannfredMortarchOfNight::s_registered = false;

    Unit *MannfredMortarchOfNight::Create(const ParameterList &parameters) {
        auto unit = new MannfredMortarchOfNight();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    int MannfredMortarchOfNight::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void MannfredMortarchOfNight::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Mannfred, Mortarch of Night", factoryMethod);
        }
    }

    MannfredMortarchOfNight::MannfredMortarchOfNight() :
            LegionOfNagashBase("Mannfred, Mortarch of Night", 16, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_gheistvor(Weapon::Type::Melee, "Gheistvor", 1, 4, 3, 3, -1, RAND_D3),
            m_glaive(Weapon::Type::Melee, "Sickle-glaive", 2, 2, 3, 3, -1, 2),
            m_ebonClaws(Weapon::Type::Melee, "Ashigaroth's Ebon Claws", 1, 6, 4, 3, -2, 2),
            m_clawsAndDaggers(Weapon::Type::Melee, "Spirits' Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, DEATHLORDS, MONSTER, HERO, WIZARD, MORTARCH, MANNFRED};
        m_weapons = {&m_gheistvor, &m_glaive, &m_ebonClaws, &m_clawsAndDaggers};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_ebonClaws.setMount(true);
        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    void MannfredMortarchOfNight::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_gheistvor);
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_ebonClaws);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    void MannfredMortarchOfNight::onWounded() {
        Unit::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_ebonClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void MannfredMortarchOfNight::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t MannfredMortarchOfNight::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds MannfredMortarchOfNight::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target,
                                                 int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_clawsAndDaggers.name())) return {0, 1};
        return LegionOfNagashBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void MannfredMortarchOfNight::onStartHero(PlayerId player) {
        LegionOfNagashBase::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(4, 18.0);
    }

    void MannfredMortarchOfNight::onEndCombat(PlayerId player) {
        // Feaster of Souls
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(2);

        LegionOfNagashBase::onEndCombat(player);
    }

} // namespace Death
