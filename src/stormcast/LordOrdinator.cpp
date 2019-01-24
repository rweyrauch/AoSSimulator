/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordOrdinator.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordOrdinator::Create,
    LordOrdinator::ValueToString,
    LordOrdinator::EnumStringToInt,
    {
        {ParamType::Integer, "weapons", {.m_intValue = LordOrdinator::AstralHammers}, LordOrdinator::AstralHammers,
         LordOrdinator::AstralGrandhammer, 1},
    }
};

bool LordOrdinator::s_registered = false;

Weapon LordOrdinator::s_astralHammers(Weapon::Type::Melee, "Astral Hammers", 1, 6, 4, 3, 0, 1);
Weapon LordOrdinator::s_astralGrandhammer(Weapon::Type::Melee, "Astral Grandhammer", 1, 3, 3, 3, -1, 2);

LordOrdinator::LordOrdinator() :
    StormcastEternal("Lord-Ordinator", 5, WOUNDS, 9, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, LORD_ORDINATOR };
}

bool LordOrdinator::configure(LordOrdinator::WeaponOption weaponOption)
{
    m_weaponOption = weaponOption;

    Model model(BASESIZE, WOUNDS);
    if (m_weaponOption == AstralHammers)
    {
        model.addMeleeWeapon(&s_astralHammers);
    }
    else
    {
        model.addMeleeWeapon(&s_astralGrandhammer);
    }
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_astralHammers.name() << ": " << s_astralHammers.strength() << std::endl;
        std::cout << "\t" << s_astralGrandhammer.name() << ": " << s_astralGrandhammer.strength() << std::endl;
    }

    return true;
}

Unit *LordOrdinator::Create(const ParameterList &parameters)
{
    auto unit = new LordOrdinator();
    WeaponOption weapons = (WeaponOption)GetIntParam("weapons", parameters, false);

    bool ok = unit->configure(weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordOrdinator::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Ordinator", factoryMethod);
    }
}

std::string LordOrdinator::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == AstralHammers)
            return "AstralHammers";
        else if (parameter.m_intValue == AstralGrandhammer)
            return "AstralGrandhammer";
    }

    return ParameterValueToString(parameter);
}

int LordOrdinator::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "AstralHammers")
        return AstralHammers;
    else if (enumString == "AstralGrandhammer")
        return AstralGrandhammer;

    return 0;
}

} // namespace StormcastEternals