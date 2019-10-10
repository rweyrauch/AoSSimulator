/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageOrruks.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageOrruks::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageOrruks::s_registered = false;

Unit *SavageOrruks::Create(const ParameterList &parameters)
{
    return nullptr;
}

void SavageOrruks::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Orruks", factoryMethod);
    }
}

SavageOrruks::SavageOrruks() :
    Unit("Savage Orruks", 5, WOUNDS, 5, 6, false),
    m_chompa(Weapon::Type::Melee, "Chompa", 1, 2, 4, 3, 0, 1),
    m_stikka(Weapon::Type::Melee, "Savage Stikka", 2, 2, 4, 4, 0, 1),
    m_chompaBoss(Weapon::Type::Melee, "Chompa (Boss)", 1, 3, 4, 3, 0, 1),
    m_stikkaBoss(Weapon::Type::Melee, "Savage Stikka (Boss)", 2, 3, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUKS};
}

bool SavageOrruks::configure(int numModels, bool skullThumper, bool totemBearer)
{
    return false;
}

void SavageOrruks::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_chompa);
    visitor(&m_chompaBoss);
    visitor(&m_stikka);
    visitor(&m_stikkaBoss);
}

} // namespace Bonesplitterz
