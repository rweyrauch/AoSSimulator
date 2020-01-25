/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/IronGolems.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    IronGolems::Create,
    nullptr,
    nullptr,
    IronGolems::ComputePoints,
    {
        {
            ParamType::Integer, "Models", IronGolems::MIN_UNIT_SIZE, IronGolems::MIN_UNIT_SIZE,
            IronGolems::MAX_UNIT_SIZE, IronGolems::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
    },
    CHAOS,
    { SLAVES_TO_DARKNESS }
};

bool IronGolems::s_registered = false;

Unit *IronGolems::Create(const ParameterList &parameters)
{
    auto unit = new IronGolems();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void IronGolems::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Iron Golems", factoryMethod);
    }
}

IronGolems::IronGolems() :
    SlavesToDarknessBase("Iron Golems", 5, WOUNDS, 6, 4, false),
    m_bolas(Weapon::Type::Missile, "Bolas", 8, 1, 4, 4, 0, 1),
    m_legionWeapons(Weapon::Type::Melee, "Legion Weapons", 1, 1, 4, 4, 0, 1),
    m_legionWeaponsDominar(Weapon::Type::Melee, "Legion Weapons (Dominar)", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, IRON_GOLEMS};
    m_weapons = {&m_bolas, &m_legionWeapons, &m_legionWeaponsDominar};
}

bool IronGolems::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto dominar = new Model(BASESIZE, WOUNDS);
    dominar->addMissileWeapon(&m_bolas);
    dominar->addMeleeWeapon(&m_legionWeaponsDominar);
    dominar->setName("Dominar");
    addModel(dominar);

    auto signifer = new Model(BASESIZE, WOUNDS);
    signifer->addMissileWeapon(&m_bolas);
    signifer->addMeleeWeapon(&m_legionWeapons);
    signifer->setName("Signifer");
    addModel(signifer);

    auto breacher = new Model(BASESIZE, 3);
    breacher->addMissileWeapon(&m_bolas);
    breacher->addMeleeWeapon(&m_legionWeapons);
    breacher->setName("Ogor Breacher");
    addModel(breacher);

    for (auto i = 3; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_bolas);
        model->addMeleeWeapon(&m_legionWeapons);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int IronGolems::braveryModifier() const
{
    int modifier = Unit::braveryModifier();
    if (m_hasSignifer)
    {
        modifier += 2;
    }
    return modifier;
}

void IronGolems::onWounded()
{
    Unit::onWounded();

    // Check for Signifer
    for (const auto& ip : m_models)
    {
        if (ip->slain() && (ip->getName() == "Signifer"))
        {
            m_hasSignifer = false;
            break;
        }
    }

}

int IronGolems::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //SlavesToDarkness