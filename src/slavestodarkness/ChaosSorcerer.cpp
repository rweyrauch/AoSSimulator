/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosSorcerer.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosSorcerer::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    { SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE }
};

bool ChaosSorcerer::s_registered = false;

Unit *ChaosSorcerer::Create(const ParameterList &parameters)
{
    auto unit = new ChaosSorcerer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosSorcerer::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Sorcerer", factoryMethod);
    }
}

ChaosSorcerer::ChaosSorcerer() :
    Unit("Chaos Sorcerer", 5, WOUNDS, 7, 4, false),
    m_staff(Weapon::Type::Melee, "Sorcerous Staff", 2, 1, 4, 3, -1, RAND_D3),
    m_blade(Weapon::Type::Melee, "Chaos Runeblade", 1, 2, 3, 3, 0, 1)
{
    m_keywords = { CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, WIZARD, CHAOS_SORCERER_LORD };
    m_weapons = {&m_staff, &m_blade};
}

bool ChaosSorcerer::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_blade);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //namespace SlavesToDarkness
