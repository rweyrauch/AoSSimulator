/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/NeaveBlacktalon.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    NeaveBlacktalon::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool NeaveBlacktalon::s_registered = false;

NeaveBlacktalon::NeaveBlacktalon() :
    StormcastEternal("Neave Blacktalon", 6, WOUNDS, 9, 3, false),
    m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 3, 0, 1),
    m_whirlwindAxes(Weapon::Type::Melee, "The Whirlwind Axes", 1, 7, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, HERO, KNIGHT_ZEPHYROS, NEAVE_BLACKTALON};
    // Tireless hunder
    m_runAndShoot = true;
}

bool NeaveBlacktalon::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_boltstormPistol);
    model.addMeleeWeapon(&m_whirlwindAxes);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *NeaveBlacktalon::Create(const ParameterList &parameters)
{
    auto unit = new NeaveBlacktalon();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void NeaveBlacktalon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Neave Blacktalon", factoryMethod);
    }
}

int NeaveBlacktalon::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Lightning-fast Strikes
    int attacks = StormcastEternal::extraAttacks(nullptr, weapon, target);
    if (charged())
    {
        attacks += 1;
    }
    return attacks;
}

void NeaveBlacktalon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_boltstormPistol);
    visitor(&m_whirlwindAxes);
}

Wounds NeaveBlacktalon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Nemesis
    if (target->hasKeyword(HERO))
    {
        return {weapon->damage()+1, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace StormcastEternals