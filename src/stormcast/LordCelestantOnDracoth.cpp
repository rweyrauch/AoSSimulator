/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestantOnDracoth.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordCelestantOnDracoth::Create,
    LordCelestantOnDracoth::ValueToString,
    {
        {ParamType::Integer, "weapons", {.m_intValue = LordCelestantOnDracoth::TempestosHammer}, LordCelestantOnDracoth::TempestosHammer,
         LordCelestantOnDracoth::StormstrikeGlaive, 1},
    }
};

bool LordCelestantOnDracoth::s_registered = false;

Weapon LordCelestantOnDracoth::s_stormstrikeGlaive(Weapon::Type::Melee, "Stormstrike Glaive", 2, 4, 3, 4, -1, 1);
Weapon LordCelestantOnDracoth::s_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2);
Weapon LordCelestantOnDracoth::s_thunderaxe(Weapon::Type::Melee, "Thunderaxe", 2, 3, 3, 3, -1, 2);
Weapon LordCelestantOnDracoth::s_tempestosHammer(Weapon::Type::Melee, "Tempestos Hammer", 2, 3, 3, 2, -1, RAND_D3);
Weapon LordCelestantOnDracoth::s_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1);

LordCelestantOnDracoth::LordCelestantOnDracoth() :
    StormcastEternal("Lord-Celestant on Dracoth", 10, WOUNDS, 9, 3, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, LORD_CELESTANT };
}

bool LordCelestantOnDracoth::configure(WeaponOption weapons)
{
    m_weapon = weapons;

    Model model(BASESIZE, WOUNDS);
    if (m_weapon == StormstrikeGlaive)
        model.addMeleeWeapon(&s_stormstrikeGlaive);
    else if (m_weapon == LightningHammer)
        model.addMeleeWeapon(&s_lightningHammer);
    else if (m_weapon == Thunderaxe)
        model.addMeleeWeapon(&s_thunderaxe);
    else if (m_weapon == TempestosHammer)
        model.addMeleeWeapon(&s_tempestosHammer);

    model.addMeleeWeapon(&s_clawsAndFangs);
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_stormstrikeGlaive.name() << ": " << s_stormstrikeGlaive.strength() << std::endl;
        std::cout << "\t" << s_lightningHammer.name() << ": " << s_lightningHammer.strength() << std::endl;
        std::cout << "\t" << s_thunderaxe.name() << ": " << s_thunderaxe.strength() << std::endl;
        std::cout << "\t" << s_tempestosHammer.name() << ": " << s_tempestosHammer.strength() << std::endl;
        std::cout << "\t" << s_clawsAndFangs.name() << ": " << s_clawsAndFangs.strength() << std::endl;
    }

    return true;
}

Unit *LordCelestantOnDracoth::Create(const ParameterList &parameters)
{
    auto unit = new LordCelestantOnDracoth();
    auto weapons = (WeaponOption)GetIntParam("weapons", parameters, LightningHammer);

    bool ok = unit->configure(weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordCelestantOnDracoth::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Celestant on Dracoth", factoryMethod);
    }
}

std::string LordCelestantOnDracoth::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == TempestosHammer)
            return "TempestosHammer";
        else if (parameter.m_intValue == Thunderaxe)
            return "Thunderaxe";
        else if (parameter.m_intValue == LightningHammer)
            return "LightningHammer";
        else if (parameter.m_intValue == StormstrikeGlaive)
            return "StormstrikeGlaive";
    }
    return ParameterValueToString(parameter);
}

} // namespace StormcastEternals