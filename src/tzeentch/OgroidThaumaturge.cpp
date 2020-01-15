/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/OgroidThaumaturge.h"

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    OgroidThaumaturge::Create,
    OgroidThaumaturge::ValueToString,
    OgroidThaumaturge::EnumStringToInt,
    OgroidThaumaturge::ComputePoints,
    {
    },
    CHAOS,
    { TZEENTCH }
};

bool OgroidThaumaturge::s_registered = false;

Unit *OgroidThaumaturge::Create(const ParameterList &parameters)
{
    auto unit = new OgroidThaumaturge();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string OgroidThaumaturge::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int OgroidThaumaturge::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void OgroidThaumaturge::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ogroid Thaumaturge", factoryMethod);
    }
}

OgroidThaumaturge::OgroidThaumaturge() :
    Unit("OgroidT haumaturge", 6, WOUNDS, 8, 5, false),
    m_staff(Weapon::Type::Melee, "Thaumaturge Staff", 2, 3, 3, 3, -1, RAND_D3),
    m_horns(Weapon::Type::Melee, "Great Horns", 1, 2, 3, 3, -2, 3),
    m_hooves(Weapon::Type::Melee, "Cloven Hooves", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, OGROID_THAUMATURGE};
    m_weapons = {&m_staff, &m_horns, &m_hooves};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool OgroidThaumaturge::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_hooves);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // Tzeentch