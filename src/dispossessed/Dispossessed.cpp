/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Dispossessed.h>

namespace Dispossessed
{

void Dispossessed::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const
{
    // Stubborn to the End
    if (roll <= 3)
    {
        numFled = 0;
        numAdded = 0;
    }
    else
    {
        Unit::computeBattleshockEffect(roll, numFled, numAdded);
    }
}

Rerolls Dispossessed::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    switch (m_grudge)
    {
        case StuckUp:
            if (target->hasKeyword(HERO))
            {
                return RerollOnes;
            }
            break;
        case SpeedMerchants:
            if (target->move() >= 10)
            {
                return RerollOnes;
            }
            break;
        case MonstrousCheaters:
            if (target->hasKeyword(MONSTER))
            {
                return RerollOnes;
            }
            break;
        case CowardlyHorders:
            if (target->initialModels() >= 20)
            {
                return RerollOnes;
            }
            break;
        case ShoddyCraftsmanship:
            if (target->save() >= 2 && target->save() <= 4)
            {
                return RerollOnes;
            }
            break;
        case SneakyAmbushers:
            // TODO:
            break;
    }
    return Unit::toHitRerolls(weapon, target);
}

} //namespace Dispossessed