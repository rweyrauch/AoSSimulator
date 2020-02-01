/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/LadyOlynder.h"

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    LadyOlynder::Create,
    nullptr,
    nullptr,
    LadyOlynder::ComputePoints,
    {
    },
    DEATH,
    { NIGHTHAUNT }
};

bool LadyOlynder::s_registered = false;

Unit *LadyOlynder::Create(const ParameterList &parameters)
{
    auto unit = new LadyOlynder();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LadyOlynder::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lady Olynder", factoryMethod);
    }
}

LadyOlynder::LadyOlynder() :
    Nighthaunt("Lady Olynder", 6, WOUNDS, 10, 4, true),
    m_staff(Weapon::Type::Melee, "Staff of Midnight", 2, 3, 3, 3, -2, RAND_D3),
    m_claws(Weapon::Type::Melee, "Banshee Handmaidens' Spectral Claws", 1, 6, 4, 4, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, MORTARCH, LADY_OLYNDER};
    m_weapons = {&m_staff, &m_claws};
}

bool LadyOlynder::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_claws);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

} // namespace Nighthaunt
