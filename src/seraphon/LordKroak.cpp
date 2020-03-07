/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/LordKroak.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    LordKroak::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    LordKroak::ComputePoints,
    {},
    ORDER,
    { SERAPHON }
};

bool LordKroak::s_registered = false;

LordKroak::LordKroak() :
    SeraphonBase("Lord Kroak", 5, WOUNDS, 9, 4, true),
    m_barrier(Weapon::Type::Melee, "Azyrite Force Barrier", 3, 1, 3, 3, -1, 1)
{
    m_keywords = {ORDER, SERAPHON, SLANN, HERO, WIZARD, STARMASTER, LORD_KROAK};
    m_weapons = {&m_barrier};
}

bool LordKroak::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_barrier);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *LordKroak::Create(const ParameterList &parameters)
{
    auto unit = new LordKroak();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordKroak::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord Kroak", factoryMethod);
    }
}

} //namespace Seraphon
