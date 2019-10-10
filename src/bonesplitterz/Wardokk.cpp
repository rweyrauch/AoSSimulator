/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/Wardokk.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    Wardokk::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool Wardokk::s_registered = false;

Unit *Wardokk::Create(const ParameterList &parameters)
{
    return nullptr;
}

void Wardokk::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Wardokk", factoryMethod);
    }
}

Wardokk::Wardokk() :
    Unit("Wardokk", 5, WOUNDS, 7, 6, false),
    m_bonebeastStikk(Weapon::Type::Melee, "Bonebeast Stikk", 1, 1, 4, 3, 0, RAND_D3)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, PRIEST, WIZARD, WARDOKK};
}

bool Wardokk::configure()
{
    return false;
}

void Wardokk::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bonebeastStikk);
}

} // namespace Bonesplitterz
