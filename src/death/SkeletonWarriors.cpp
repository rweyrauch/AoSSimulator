/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/SkeletonWarriors.h>

namespace Death
{

SkeletonWarriors::SkeletonWarriors()
{

}

bool SkeletonWarriors::configure(int numModels, SkeletonWarriors::WeaponOptions weapons, bool standardBearers, bool hornblowers)
{
    return false;
}

void SkeletonWarriors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

Unit *SkeletonWarriors::Create(const ParameterList &parameters)
{
    return nullptr;
}

void SkeletonWarriors::Init()
{

}

} //namespace Death
