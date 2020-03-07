/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkStarpriest.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SkinkStarpriest::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    SkinkStarpriest::ComputePoints,
    {},
    ORDER,
    { SERAPHON }
};

bool SkinkStarpriest::s_registered = false;

SkinkStarpriest::SkinkStarpriest() :
    SeraphonBase("Skink Starpriest", 8, WOUNDS, 6, 5, false),
    m_venombolt(Weapon::Type::Missile, "Venombolt", 18, 2, 3, 3, -1, 1),
    m_staff(Weapon::Type::Melee, "Serpent Staff", 1, 2, 4, 3, -1, 1)
{
    m_keywords = {ORDER, SERAPHON, SKINK, HERO, WIZARD, STARPRIEST};
    m_weapons = {&m_venombolt, &m_staff};
}

bool SkinkStarpriest::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_venombolt);
    model->addMeleeWeapon(&m_staff);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SkinkStarpriest::Create(const ParameterList &parameters)
{
    auto unit = new SkinkStarpriest();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SkinkStarpriest::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skink Starpriest", factoryMethod);
    }
}

} //namespace Seraphon