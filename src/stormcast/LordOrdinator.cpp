/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordOrdinator.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordOrdinator::Create,
    {
        {ParamType::Integer, "weaponOption", {.m_intValue = LordOrdinator::AstralHammers}, LordOrdinator::AstralHammers,
         LordOrdinator::AstralGrandhammer},
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

    return true;
}

Unit *LordOrdinator::Create(const ParameterList &parameters)
{
    auto unit = new LordOrdinator();
    WeaponOption weapons = (WeaponOption)GetIntParam("weaponOption", parameters, false);

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

} // namespace StormcastEternals