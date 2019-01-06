/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/Bloodreavers.h>

namespace Khorne
{

Weapon Bloodreavers::s_reaverBlades("Reaver Blades", 1, 1, 4, 4, 0, 1);
Weapon Bloodreavers::s_reaverBladesChieftain("Reaver Blades", 1, 2, 4, 4, 0, 1);
Weapon Bloodreavers::s_meatripperAxe("Meatripper Axe", 1, 1, 4, 4, -1, 1);
Weapon Bloodreavers::s_meatripperAxeChieftain("Meatripper Axe", 1, 2, 4, 4, -1, 1);

std::vector<std::string> Bloodreavers::s_keywords =
{
    "CHAOS", "MORTAL", "KHORNE", "BLOODBOUND", "BLOODREAVERS"
};

Bloodreavers::Bloodreavers() :
    Unit("Bloodreavers", 6, WOUNDS, 5, 6, false)
{
}


bool Bloodreavers::configure(int numModels, Bloodreavers::WeaponOption weapons, bool iconBearer, bool hornblowers)
{
    if (numModels < 10 || numModels > 30)
    {
        return false;
    }

    m_weaponOption = weapons;
    m_iconBearer = iconBearer;
    m_hornblower = hornblowers;

    // Add the Chieftain
    Model chieftainModel(BASESIZE, WOUNDS);
    if (m_weaponOption == ReaverBlades)
    {
        chieftainModel.addMeleeWeapon(s_reaverBladesChieftain);
    }
    else if (m_weaponOption == MeatripperAxe)
    {
        chieftainModel.addMeleeWeapon(s_meatripperAxeChieftain);
    }
    addModel(chieftainModel);

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == ReaverBlades)
            model.addMeleeWeapon(s_reaverBlades);
        else if (m_weaponOption == MeatripperAxe)
            model.addMeleeWeapon(s_meatripperAxe);
        addModel(model);
    }

    return true;
}

bool Bloodreavers::hasKeyword(const std::string& word) const
{
    auto kw = std::find(s_keywords.begin(), s_keywords.end(), word);
    return (kw != s_keywords.end());
}

Rerolls Bloodreavers::toHitRerolls(const Unit &unit) const
{
    // Reaver Blades
    if (m_weaponOption == ReaverBlades)
        return RerollOnes;

    return Unit::toHitRerolls(unit);
}

} // namespace Khorne