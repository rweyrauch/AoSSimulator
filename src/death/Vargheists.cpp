/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Vargheists.h>

namespace Death
{

Vargheists::Vargheists()
{

}

bool Vargheists::configure(int numModels)
{
    return false;
}

void Vargheists::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

Unit *Vargheists::Create(const ParameterList &parameters)
{
    return nullptr;
}

void Vargheists::Init()
{

}
} //namespace Death
