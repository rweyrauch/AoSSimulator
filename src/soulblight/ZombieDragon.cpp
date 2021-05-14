/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <soulblight/ZombieDragon.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 130;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_breathToWound;
        int m_clawsAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 2, 7},
                    {12, 3, 6},
                    {10, 4, 5},
                    {8,  5, 4},
                    {6,  6, 3}
            };

    bool ZombieDragon::s_registered = false;

    ZombieDragon::ZombieDragon(CursedBloodline legion) :
            SoulblightBase(legion, "Zombie Dragon", 14, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_pestilentialBreath(Weapon::Type::Missile, "Pestilential Breath", 9, 1, 3, 2, -3, RAND_D6),
            m_snappingMaw(Weapon::Type::Melee, "Snapping Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_swordlikeClaws(Weapon::Type::Melee, "Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, MENAGERIE, MONSTER, ROYAL_ZOMBIE_DRAGON};
        m_weapons = {&m_pestilentialBreath, &m_snappingMaw, &m_swordlikeClaws};
        m_battleFieldRole = Role::Behemoth;

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_pestilentialBreath);
        model->addMeleeWeapon(&m_snappingMaw);
        model->addMeleeWeapon(&m_swordlikeClaws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *ZombieDragon::Create(const ParameterList &parameters) {
        return nullptr;
    }

    void ZombieDragon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ZombieDragon::Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ZombieDragon::ComputePoints,
                    {
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Zombie Dragon", factoryMethod);
        }
    }

    size_t ZombieDragon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ZombieDragon::onWounded() {
        SoulblightBase::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_pestilentialBreath.setToWound(g_damageTable[damageIndex].m_breathToWound);
        m_swordlikeClaws.setAttacks(g_damageTable[damageIndex].m_clawsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void ZombieDragon::onRestore() {
        SoulblightBase::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    int ZombieDragon::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int ZombieDragon::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SoulblightBase::toHitModifier(weapon, target);

        // Pestilential Breath
        if ((weapon->name() == m_pestilentialBreath.name())) {
            if (Dice::RollD6() <= target->remainingModels()) {
                // Auto hits
                mod += 6;
            }
        }
        return mod;
    }

} // namespace FleshEasterCourt
