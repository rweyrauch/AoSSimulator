/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightZephyros.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    KnightZephyros::Create,
    nullptr,
    nullptr,
    {
    }
};

bool KnightZephyros::s_registered = false;

KnightZephyros::KnightZephyros() :
    StormcastEternal("Knight-Zephyros", 6, WOUNDS, 9, 3, false),
    m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 3, 0, 1),
    m_tempestAxes(Weapon::Type::Melee, "Tempest Axe", 1, 6, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, KNIGHT_ZEPHYROS};
    // Tireless hunder
    m_runAndShoot = true;
}

bool KnightZephyros::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_boltstormPistol);
    model.addMeleeWeapon(&m_tempestAxes);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *KnightZephyros::Create(const ParameterList &parameters)
{
    auto unit = new KnightZephyros();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KnightZephyros::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Knight-Zephyros", factoryMethod);
    }
}

int KnightZephyros::extraAttacks(const Weapon *weapon) const
{
    // Lightning-fast Strikes
    int attacks = Unit::extraAttacks(weapon);
    if (charged())
    {
        attacks += 1;
    }
    return attacks;
}

void KnightZephyros::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_boltstormPistol);
    visitor(&m_tempestAxes);
}

} // namespace StormcastEternals