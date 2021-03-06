/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Neferata.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 365;

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

    bool NeferataMortarchOfBlood::s_registered = false;

    Unit *NeferataMortarchOfBlood::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new NeferataMortarchOfBlood(lore, general);
    }

    int NeferataMortarchOfBlood::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void NeferataMortarchOfBlood::Init() {
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
                    {SOULBLIGHT_GRAVELORDS, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Neferata, Mortarch of Blood", factoryMethod);
        }
    }

    NeferataMortarchOfBlood::NeferataMortarchOfBlood(Lore lore, bool isGeneral) :
            SoulblightBase(CursedBloodline::Legion_Of_Blood, "Neferata, Mortarch of Blood", 16, g_wounds, 10, 3, true, g_pointsPerUnit),
            m_akmetHar(Weapon::Type::Melee, "Akmet-har", 1, 5, 2, 3, -1, 1),
            m_akenSeth(Weapon::Type::Melee, "Aken-seth", 1, 2, 2, 3, -2, 2),
            m_skeletalClaws(Weapon::Type::Melee, "Abyssal Talons", 1, 6, 4, 3, -2, 2),
            m_clawsAndDaggers(Weapon::Type::Melee, "Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, DEATHLORDS, LEGION_OF_BLOOD, MONSTER, HERO, WIZARD, MORTARCH, NEFERATA};
        m_weapons = {&m_akmetHar, &m_akenSeth, &m_skeletalClaws, &m_clawsAndDaggers};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_skeletalClaws.setMount(true);
        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_akmetHar);
        model->addMeleeWeapon(&m_akenSeth);
        model->addMeleeWeapon(&m_skeletalClaws);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void NeferataMortarchOfBlood::onWounded() {
        SoulblightBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_skeletalClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void NeferataMortarchOfBlood::onRestore() {
        SoulblightBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t NeferataMortarchOfBlood::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds NeferataMortarchOfBlood::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target,
                                                 int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_clawsAndDaggers.name())) return {0, 1};
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void NeferataMortarchOfBlood::onEndCombat(PlayerId player) {
        // Mortarch of Blood
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(RAND_D6);

        SoulblightBase::onEndCombat(player);
    }

} // namespace Soulblight
