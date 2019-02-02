/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Zombies.h>

namespace Death
{

Zombies::Zombies()
{

}

bool Zombies::configure(int numModels)
{
    return false;
}

void Zombies::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

Unit *Zombies::Create(const ParameterList &parameters)
{
    return nullptr;
}

void Zombies::Init()
{

}

} //namespace Death
