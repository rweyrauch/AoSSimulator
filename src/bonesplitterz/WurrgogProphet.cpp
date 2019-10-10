/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/WurrgogProphet.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    WurrgogProphet::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool WurrgogProphet::s_registered = false;

Unit *WurrgogProphet::Create(const ParameterList &parameters)
{
    return nullptr;
}

void WurrgogProphet::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Wurrgog Prophet", factoryMethod);
    }
}

WurrgogProphet::WurrgogProphet() :
    Unit("Wurrgog Prophet", 5, WOUNDS, 8, 5, false),
    m_staffAndShiv(Weapon::Type::Melee, "Wurrgog Staff and Bone Shiv", 1, 4, 4, 3, 0, RAND_D3),
    m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 1, RAND_D3, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, WIZARD, WURRGOG_PROPHET};
}

bool WurrgogProphet::configure()
{
    return false;
}

void WurrgogProphet::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_staffAndShiv);
    visitor(&m_fangedMaw);
}

} // namespace Bonesplitterz
