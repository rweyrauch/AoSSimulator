/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/VampireLordOnDragon.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 130;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 440;

    struct TableEntry {
        int m_move;
        int m_breathToWound;
        int m_clawAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 2, 7},
                    {12, 3, 6},
                    {10, 4, 5},
                    {8, 5, 4},
                    {6, 6, 3}
            };

    bool VampireLordOnZombieDragon::s_registered = false;

    Unit *VampireLordOnZombieDragon::Create(const ParameterList &parameters) {
        auto unit = new VampireLordOnZombieDragon();

        auto weapon = (WeaponOption)GetEnumParam("Weapon", parameters, Deathlance);
        bool shield = GetBoolParam("Ancient Shield", parameters, true);
        bool chalice = GetBoolParam("Chalice of Blood", parameters, true);

        auto legion = (Legion)GetEnumParam("Legion", parameters, GrandHostOfNagash);
        unit->setLegion(legion);

        bool ok = unit->configure(weapon, shield, chalice);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int VampireLordOnZombieDragon::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void VampireLordOnZombieDragon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Weapon", Deathlance, Deathlance, VampiricSword, 1},
                            {ParamType::Boolean, "Ancient Shield", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Boolean, "Chalice of Blood", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Enum, "Legion", Legion::GrandHostOfNagash, Legion ::GrandHostOfNagash, Legion::LegionOfBlood, 1},
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Vampire Lord on Zombie Dragon", factoryMethod);
        }
    }

    VampireLordOnZombieDragon::VampireLordOnZombieDragon() :
            LegionOfNagashBase("Vampire Lord on Zombie Dragon", 14, WOUNDS, 10, 4, true),
            m_breath(Weapon::Type::Missile, "Pestilential Breath", 9, 1, 3, 2, -3, RAND_D6),
            m_deathlance(Weapon::Type::Melee, "Deathlance", 1, 3, 3, 3, -1, 2),
            m_sword(Weapon::Type::Melee, "Vampiric Sword", 1, 4, 3, 3, -1, RAND_D3),
            m_maw(Weapon::Type::Melee, "Zombie Dragon's Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Zombie Dragon's Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, SOULBLIGHT, ZOMBIE_DRAGON, MONSTER, HERO, WIZARD, VAMPIRE_LORD, VAMPIRE};
        m_weapons = {&m_breath, &m_deathlance, &m_maw, &m_claws};
    }

    bool VampireLordOnZombieDragon::configure(WeaponOption option, bool shield, bool chalice) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_breath);

        if (option == Deathlance) {
            model->addMeleeWeapon(&m_deathlance);
        } else if (option == VampiricSword) {
            model->addMeleeWeapon(&m_sword);
        }
        model->addMeleeWeapon(&m_maw);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        // Ancient Shield
        if (shield) {
            m_save = 3;
        }

        m_haveChaliceOfBlood = chalice;

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void VampireLordOnZombieDragon::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_breath.setToWound(g_damageTable[damageIndex].m_breathToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void VampireLordOnZombieDragon::onRestore() {
        Unit::onRestore();

        m_usedChaliceOfBlood = false;

        // Restore table-driven attributes
        onWounded();
    }

    int VampireLordOnZombieDragon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void VampireLordOnZombieDragon::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {

            deathlyInvocations();

            // Chalice of Blood
            if (m_haveChaliceOfBlood && !m_usedChaliceOfBlood && remainingWounds() < wounds()) {
                heal(Dice::rollD6());
                m_usedChaliceOfBlood = true;
            }
        }
    }

    Wounds VampireLordOnZombieDragon::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(1);

        return Unit::onEndCombat(player);
    }

    Wounds VampireLordOnZombieDragon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                   int woundRoll) const {
        // Deathlance Charge
        if (m_charged && (weapon->name() == m_deathlance.name())) return { 3, 0};
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }
} // namespace Death