/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/ManiakWeirdnob.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    ManiakWeirdnob::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool ManiakWeirdnob::s_registered = false;

Unit *ManiakWeirdnob::Create(const ParameterList &parameters)
{
    return nullptr;
}

void ManiakWeirdnob::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Maniak Weirdnob", factoryMethod);
    }
}

ManiakWeirdnob::ManiakWeirdnob() :
    Unit("Maniak Weirdnob", 12, WOUNDS, 7, 6, false),
    m_bonebeastStaff(Weapon::Type::Melee, "Bonebeast Staff", 1, 3, 4, 3, 0, RAND_D3),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, WIZARD, MANIAK_WEIRDNOB};
}

bool ManiakWeirdnob::configure()
{
    return false;
}

void ManiakWeirdnob::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bonebeastStaff);
    visitor(&m_tusksAndHooves);
}

} // namespace Bonesplitterz
