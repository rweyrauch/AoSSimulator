/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageBigBoss.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageBigBoss::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageBigBoss::s_registered = false;

Unit *SavageBigBoss::Create(const ParameterList &parameters)
{
    return nullptr;
}

void SavageBigBoss::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Big Boss", factoryMethod);
    }
}

SavageBigBoss::SavageBigBoss() :
    Unit("Savage Big Boss", 5, WOUNDS, 7, 6, false),
    m_chompasBoss(Weapon::Type::Melee, "Boss Chompa", 1, 6, 3, 3, -1, 2)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, SAVAGE_BIG_BOSS};
}

bool SavageBigBoss::configure()
{
    return false;
}

void SavageBigBoss::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_chompasBoss);
}

} // namespace Bonesplitterz
