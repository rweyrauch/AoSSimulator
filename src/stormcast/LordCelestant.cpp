/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestant.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordCelestant::Create,
    nullptr,
    {
    }
};

bool LordCelestant::s_registered = false;

Weapon LordCelestant::s_runeblade(Weapon::Type::Melee, "Sigmarite Runeblade", 1, 4, 3, 3, -1, 1);
Weapon LordCelestant::s_warhammer(Weapon::Type::Melee, "Warhammer", 1, 2, 4, 3, 0, 1);

LordCelestant::LordCelestant() :
    StormcastEternal("Lord-Celestant", 5, WOUNDS, 9, 3, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, LORD_CELESTANT };
}

bool LordCelestant::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&s_runeblade);
    model.addMeleeWeapon(&s_warhammer);
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_runeblade.name() << ": " << s_runeblade.strength() << std::endl;
        std::cout << "\t" << s_warhammer.name() << ": " << s_warhammer.strength() << std::endl;
    }

    return true;
}

Unit *LordCelestant::Create(const ParameterList &parameters)
{
    auto unit = new LordCelestant();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordCelestant::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Celestant", factoryMethod);
    }
}

} // namespace StormcastEternals