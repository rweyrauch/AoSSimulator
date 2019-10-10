/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageBigStabbas.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageBigStabbas::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageBigStabbas::s_registered = false;

Unit *SavageBigStabbas::Create(const ParameterList &parameters)
{
    return nullptr;
}

void SavageBigStabbas::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Big Stabbas", factoryMethod);
    }
}

SavageBigStabbas::SavageBigStabbas() :
    Unit("Savage Big Stabbas", 5, WOUNDS, 6, 6, false),
    m_gorkToof(Weapon::Type::Melee, "Gorktoof", 3, 3, 3, 3, -2, RAND_D3)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_BIG_STABBAS};
}

bool SavageBigStabbas::configure(int numModels)
{
    return false;
}

void SavageBigStabbas::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_gorkToof);
}

} // namespace Bonesplitterz
