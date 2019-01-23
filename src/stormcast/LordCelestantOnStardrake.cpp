/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestantOnStardrake.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordCelestantOnStardrake::Create,
    LordCelestantOnStardrake::ValueToString,
    {
        {ParamType::Integer, "weapons", {.m_intValue = LordCelestantOnStardrake::CelestineHammer}, LordCelestantOnStardrake::CelestineHammer,
         LordCelestantOnStardrake::StormboundBlade, 1},
    }
};

bool LordCelestantOnStardrake::s_registered = false;

Weapon LordCelestantOnStardrake::s_celestineHammer(Weapon::Type::Melee, "Celestine Hammer", 2, 3, 3, 2, -1, RAND_D3);
Weapon LordCelestantOnStardrake::s_stormboundBlade(Weapon::Type::Melee, "Stormbound Blade", 2, 3, 3, 4, -1, 2);
Weapon LordCelestantOnStardrake::s_greatClaws(Weapon::Type::Melee, "Great Claws", 1, 4, 3, 3, -1, RAND_D3);

LordCelestantOnStardrake::LordCelestantOnStardrake() :
    StormcastEternal("Lord-Celestant on Stardrake", 12, WOUNDS, 9, 3, true)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STARDRAKE, STORMCAST_ETERNAL, HERO, MONSTER, LORD_CELESTANT };
}

bool LordCelestantOnStardrake::configure(WeaponOption weapons)
{
    m_weapons = weapons;

    Model model(BASESIZE, WOUNDS);
    if (weapons == CelestineHammer)
        model.addMeleeWeapon(&s_celestineHammer);
    else if (weapons == StormboundBlade)
        model.addMeleeWeapon(&s_stormboundBlade);
    model.addMeleeWeapon(&s_greatClaws);
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_celestineHammer.name() << ": " << s_celestineHammer.strength() << std::endl;
        std::cout << "\t" << s_stormboundBlade.name() << ": " << s_stormboundBlade.strength() << std::endl;
        std::cout << "\t" << s_greatClaws.name() << ": " << s_greatClaws.strength() << std::endl;
    }

    return true;
}

Unit *LordCelestantOnStardrake::Create(const ParameterList &parameters)
{
    auto unit = new LordCelestantOnStardrake();
    auto weapons = (WeaponOption)GetIntParam("weapons", parameters, CelestineHammer);

    bool ok = unit->configure(weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordCelestantOnStardrake::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Celestant on Stardrake", factoryMethod);
    }
}

std::string LordCelestantOnStardrake::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == CelestineHammer)
            return "CelestineHammer";
        else if (parameter.m_intValue == StormboundBlade)
            return "StormboundBlade";
    }
    return ParameterValueToString(parameter);
}

} // namespace StormcastEternals