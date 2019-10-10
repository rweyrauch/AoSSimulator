/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageBoarboys.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageBoarboys::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageBoarboys::s_registered = false;

Unit *SavageBoarboys::Create(const ParameterList &parameters)
{
    return nullptr;
}

void SavageBoarboys::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Boarboys", factoryMethod);
    }
}

SavageBoarboys::SavageBoarboys() :
    Unit("Savage Boarboys", 12, WOUNDS, 5, 6, false),
    m_chompa(Weapon::Type::Melee, "Chompa", 1, 3, 4, 3, 0, 1),
    m_stikka(Weapon::Type::Melee, "Savage Stikka", 2, 3, 4, 4, 0, 1),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1),
    m_chompaBoss(Weapon::Type::Melee, "Chompa", 1, 4, 4, 3, 0, 1),
    m_stikkaBoss(Weapon::Type::Melee, "Savage Stikka (Boss)", 2, 4, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, BOARBOYS, SAVAGE_BOARBOYS};
}

bool SavageBoarboys::configure(int numModels, bool skullThumper, bool totemBearer)
{
    return false;
}

void SavageBoarboys::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_chompa);
    visitor(&m_chompaBoss);
    visitor(&m_stikka);
    visitor(&m_stikkaBoss);
    visitor(&m_tusksAndHooves);
}

} // namespace Bonesplitterz
