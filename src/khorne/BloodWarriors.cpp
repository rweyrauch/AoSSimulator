/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/BloodWarriors.h>

namespace Khorne
{

Weapon BloodWarriors::s_goreaxe("Goreaxe", 1, 2, 3, 4, 0, 1);
Weapon BloodWarriors::s_goreaxeChampion("Goreaxe", 1, 3, 3, 4, 0, 1);
Weapon BloodWarriors::s_goreglaive("Goreglaive", 1, 2, 3, 3, -1, 2);

BloodWarriors::BloodWarriors() :
    Unit("Blood Warriors", 5, WOUNDS, 6, 4, false)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, BLOOD_WARRIORS};
}


bool BloodWarriors::configure(int numModels, bool pairedGoreax, int numGoreglaives, bool iconBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    const int maxGlaives = numModels / 10;
    if (numGoreglaives > maxGlaives)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_iconBearer = iconBearer;
    m_pairedGoreaxe = pairedGoreax;

    // Add the Champion
    Model championModel(BASESIZE, WOUNDS);
    championModel.addMeleeWeapon(&s_goreaxeChampion);
    addModel(championModel);

    for (auto i = 0; i < numGoreglaives; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_goreglaive);
        addModel(model);
    }

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_goreaxe);
        addModel(model);
    }

    return true;
}

Rerolls BloodWarriors::toHitRerolls(const Weapon* weapon, const Unit* unit) const
{
    // Goreaxes
    if (m_pairedGoreaxe && (weapon->name() == s_goreaxe.name()))
        return RerollOnes;

    return Unit::toHitRerolls(weapon, unit);
}

int BloodWarriors::battlshockModifier() const
{
    int modifier = Unit::battlshockModifier();

    // Icon Bearer
    if (m_iconBearer)
        modifier++;

    return modifier;
}

} // namespace Khorne