/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Mannfred.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 380;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 6},
                    {13, 5},
                    {10, 4},
                    {7,  3},
                    {4,  2}
            };

    bool MannfredMortarchOfNight::s_registered = false;

    Unit *MannfredMortarchOfNight::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new MannfredMortarchOfNight(general);
    }

    int MannfredMortarchOfNight::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void MannfredMortarchOfNight::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Mannfred, Mortarch of Night", factoryMethod);
        }
    }

    MannfredMortarchOfNight::MannfredMortarchOfNight(bool isGeneral) :
            SoulblightBase(CursedBloodline::Legion_Of_Night, "Mannfred, Mortarch of Night", 16, g_wounds, 10, 3, true, g_pointsPerUnit) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, DEATHLORDS, LEGION_OF_NIGHT, MONSTER, HERO, WIZARD, MORTARCH, MANNFRED};
        m_weapons = {&m_gheistvor, &m_glaive, &m_ebonClaws, &m_clawsAndDaggers};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_ebonClaws.setMount(true);
        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_gheistvor);
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_ebonClaws);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void MannfredMortarchOfNight::onWounded() {
        SoulblightBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_ebonClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void MannfredMortarchOfNight::onRestore() {
        SoulblightBase::onRestore();

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
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void MannfredMortarchOfNight::onEndCombat(PlayerId player) {
        // Feaster of Souls
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(RAND_D3);

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
