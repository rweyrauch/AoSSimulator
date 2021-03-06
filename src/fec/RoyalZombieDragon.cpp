/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/RoyalZombieDragon.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
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

    bool RoyalZombieDragon::s_registered = false;

    RoyalZombieDragon::RoyalZombieDragon(GrandCourt court, Delusion delusion) :
            FleshEaterCourts(court, delusion, "Royal Zombie Dragon", 14, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_pestilentialBreath(Weapon::Type::Missile, "Pestilential Breath", 9, 1, 3, 2, -3, RAND_D6),
            m_snappingMaw(Weapon::Type::Melee, "Snapping Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_swordlikeClaws(Weapon::Type::Melee, "Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, FLESH_EATER_COURTS, MENAGERIE, MONSTER, ROYAL_ZOMBIE_DRAGON};
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

    Unit *RoyalZombieDragon::Create(const ParameterList &parameters) {
        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);
        return new RoyalZombieDragon(court, delusion);
    }

    void RoyalZombieDragon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    RoyalZombieDragon::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    RoyalZombieDragon::ComputePoints,
                    {
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Royal Zombie Dragon", factoryMethod);
        }
    }

    size_t RoyalZombieDragon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void RoyalZombieDragon::onWounded() {
        FleshEaterCourts::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_pestilentialBreath.setToWound(g_damageTable[damageIndex].m_breathToWound);
        m_swordlikeClaws.setAttacks(g_damageTable[damageIndex].m_clawsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void RoyalZombieDragon::onRestore() {
        FleshEaterCourts::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    int RoyalZombieDragon::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int RoyalZombieDragon::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = FleshEaterCourts::toHitModifier(weapon, target);

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
