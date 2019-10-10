/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/OrrukArrowBoys.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageOrrukArrowboys::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageOrrukArrowboys::s_registered = false;

Unit *SavageOrrukArrowboys::Create(const ParameterList &parameters)
{
    return nullptr;
}

void SavageOrrukArrowboys::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Orruk Arrowboys", factoryMethod);
    }
}

SavageOrrukArrowboys::SavageOrrukArrowboys() :
    Unit("Savage Orruk Arrowboys", 5, WOUNDS, 5, 6, false),
    m_stingaBow(Weapon::Type::Missile, "Stinga Bow", 18, 2, 5, 4, 0, 1),
    m_boneShiv(Weapon::Type::Melee, "Bone Shiv", 1, 1, 4, 4, 0, 1),
    m_chompa(Weapon::Type::Melee, "Chompa", 1, 3, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUK_ARROWBOYS};
}

bool SavageOrrukArrowboys::configure(int numModels, bool skullThumper, bool totemBearer)
{
    return false;
}

void SavageOrrukArrowboys::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_stingaBow);
    visitor(&m_boneShiv);
    visitor(&m_chompa);
}

} // namespace Bonesplitterz
