/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/OrrukMorboys.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageOrrukMorboys::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageOrrukMorboys::s_registered = false;

Unit *SavageOrrukMorboys::Create(const ParameterList &parameters)
{
    return nullptr;
}

void SavageOrrukMorboys::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Orruk Morboys", factoryMethod);
    }
}

SavageOrrukMorboys::SavageOrrukMorboys() :
    Unit("Savage Orruk Morboys", 5, WOUNDS, 6, 6, false),
    m_chompaAndShiv(Weapon::Type::Melee, "Chompa and Toof Shiv", 1, 3, 4, 3, 0, 1),
    m_chompaAndShivBoss(Weapon::Type::Melee, "Chompa and Toof Shiv (Boss)", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUK_MORBOYS};
}

bool SavageOrrukMorboys::configure(int numModels, bool skullThumper, bool totemBearer)
{
    return false;
}

void SavageOrrukMorboys::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_chompaAndShiv);
    visitor(&m_chompaAndShivBoss);
}

} // namespace Bonesplitterz
