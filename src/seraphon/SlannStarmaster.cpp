/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SlannStarmaster.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SlannStarmaster::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    SlannStarmaster::ComputePoints,
    {},
    ORDER,
    { SERAPHON }
};

bool SlannStarmaster::s_registered = false;

SlannStarmaster::SlannStarmaster() :
    SeraphonBase("Slann Starmaster", 5, WOUNDS, 9, 4, true),
    m_lightning(Weapon::Type::Melee, "Azure Lightning", 3, 6, 4, 3, -1, 1)
{
    m_keywords = {ORDER, SERAPHON, SLANN, HERO, WIZARD, STARMASTER};
    m_weapons = {&m_lightning};
}

bool SlannStarmaster::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_lightning);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SlannStarmaster::Create(const ParameterList &parameters)
{
    auto unit = new SlannStarmaster();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SlannStarmaster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Slann Starmaster", factoryMethod);
    }
}

} //namespace Seraphon