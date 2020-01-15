/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Fatemaster.h"

namespace Tzeentch
{

static FactoryMethod factoryMethod = {
    Fatemaster::Create,
    Fatemaster::ValueToString,
    Fatemaster::EnumStringToInt,
    Fatemaster::ComputePoints,
    {
    },
    CHAOS,
    { TZEENTCH }
};

bool Fatemaster::s_registered = false;

Unit *Fatemaster::Create(const ParameterList &parameters)
{
    auto unit = new Fatemaster();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Fatemaster::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int Fatemaster::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Fatemaster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Fatemaster", factoryMethod);
    }
}

Fatemaster::Fatemaster() :
    Unit("Fatemaster", 16, WOUNDS, 8, 4, true),
    m_glaive(Weapon::Type::Melee, "Fireglaive of Tzeentch", 2, 3, 3, 4, 0, RAND_D3),
    m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, FATEMASTER};
    m_weapons = {&m_glaive, &m_teethAndHorns};
}

bool Fatemaster::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_glaive);
    model->addMeleeWeapon(&m_teethAndHorns);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // Tzeentch