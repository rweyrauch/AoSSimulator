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
    Bonesplitterz::ValueToString,
    Bonesplitterz::EnumStringToInt,
    {
        {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz, Bonesplitterz::Icebone, 1},
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool ManiakWeirdnob::s_registered = false;

Unit *ManiakWeirdnob::Create(const ParameterList &parameters)
{
    auto unit = new ManiakWeirdnob();

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
    unit->setWarclan(warclan);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ManiakWeirdnob::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Maniak Weirdnob", factoryMethod);
    }
}

ManiakWeirdnob::ManiakWeirdnob() :
    Bonesplitterz("Maniak Weirdnob", 12, WOUNDS, 7, 6, false),
    m_bonebeastStaff(Weapon::Type::Melee, "Bonebeast Staff", 1, 3, 4, 3, 0, RAND_D3),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, WIZARD, MANIAK_WEIRDNOB};
}

bool ManiakWeirdnob::configure()
{
        auto model = new Model(BASESIZE, WOUNDS);

    model->addMeleeWeapon(&m_bonebeastStaff);
    model->addMeleeWeapon(&m_tusksAndHooves);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ManiakWeirdnob::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_bonebeastStaff);
    visitor(m_tusksAndHooves);
}

} // namespace Bonesplitterz
