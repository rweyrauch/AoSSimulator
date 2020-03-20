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

    bool VampireLordOnZombieDragon::s_registered = false;

    Unit *VampireLordOnZombieDragon::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int VampireLordOnZombieDragon::ComputePoints(int numModels) {
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
        m_keywords = {DEATH, SOULBLIGHT, ZOMBIE_DRAGON, MONSTER, HERO, WIZARD, VAMPIRE_LORD};
        m_weapons = {&m_breath, &m_deathlance, &m_maw, &m_claws};
    }

    bool VampireLordOnZombieDragon::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_breath);
        model->addMeleeWeapon(&m_deathlance);
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_maw);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void VampireLordOnZombieDragon::onWounded() {
        Unit::onWounded();
    }

    void VampireLordOnZombieDragon::onRestore() {
        Unit::onRestore();
    }

    int VampireLordOnZombieDragon::getDamageTableIndex() const {
        return 0;
    }
} // namespace Death
