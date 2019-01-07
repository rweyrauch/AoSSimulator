/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/LordOrdinator.h>

namespace StormcastEternals
{

Weapon LordOrdinator::s_astralHammers("Astral Hammers", 1, 6, 4, 3, 0, 1);
Weapon LordOrdinator::s_astralGrandhammer("Astral Grandhammer", 1, 3, 3, 3, -1, 2);

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
        model.addMeleeWeapon(s_astralHammers);
    }
    else
    {
        model.addMeleeWeapon(s_astralGrandhammer);
    }
    addModel(model);

    return true;
}

} // namespace StormcastEternals