/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarlockBombardier.h>
#include <UnitFactory.h>

namespace Skaven
{
bool WarlockBombardier::s_registered = false;

Unit *WarlockBombardier::Create(const ParameterList &parameters)
{
    auto unit = new WarlockBombardier();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WarlockBombardier::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Warlock Bombardier", *factoryMethod);
    }
}

WarlockBombardier::WarlockBombardier() :
    Skaventide("Warlock Bombardier", 6, WOUNDS, 5, 5, false),
    m_doomrocket(Weapon::Type::Missile, "Doomrocket", 18, 1, 4, 3, -1, RAND_D6),
    m_pole(Weapon::Type::Melee, "Firing Pole", 1, 1, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, HERO, WIZARD, WARLOCK_ENGINEER,
        WARLOCK_BOMBARDIER};
    m_weapons = {&m_doomrocket, &m_pole};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool WarlockBombardier::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_doomrocket);
    model->addMeleeWeapon(&m_pole);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void WarlockBombardier::onRestore()
{
    Unit::onRestore();

    m_moreMoreFailed = false;
}

Wounds WarlockBombardier::onEndShooting(PlayerId player)
{
    auto wounds = Unit::onEndCombat(player);

    if (m_moreMoreFailed)
    {
        Dice dice;
        Wounds overloadWounds = {0, dice.rollD6()};
        applyDamage(overloadWounds);
        wounds += overloadWounds;
        m_moreMoreFailed = false;
    }
    return wounds;
}

Wounds WarlockBombardier::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // More-more Doomrocket!
    if (weapon->name() == m_doomrocket.name())
    {
        // Decide to overcharge (without using the hitRoll)
        if (moreMore())
        {
            if (hitRoll == 1)
            {
                m_moreMoreFailed = true;
                return {0, 0};
            }

            m_moreMoreFailed = false;
            return {RAND_2D6, 0};
        }
    }

    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}
} //namespace Skaven
