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
    {
    }
};

bool KnightZephyros::s_registered = false;

Weapon KnightZephyros::s_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 3, 0, 1);
Weapon KnightZephyros::s_tempestAxes(Weapon::Type::Melee, "Tempest Axe", 1, 6, 3, 3, -1, 1);

KnightZephyros::KnightZephyros() :
    StormcastEternal("Knight-Zephyros", 6, WOUNDS, 9, 3, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, KNIGHT_ZEPHYROS };
}

bool KnightZephyros::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&s_boltstormPistol);
    model.addMeleeWeapon(&s_tempestAxes);
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_boltstormPistol.name() << ": " << s_boltstormPistol.strength() << std::endl;
        std::cout << "\t" << s_tempestAxes.name() << ": " << s_tempestAxes.strength() << std::endl;
    }

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

} // namespace StormcastEternals