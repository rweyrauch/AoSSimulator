/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/HorticulousSlimux.h"

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    HorticulousSlimux::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    HorticulousSlimux::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

bool HorticulousSlimux::s_registered = false;

Unit* HorticulousSlimux::Create(const ParameterList &parameters)
{
    auto unit = new HorticulousSlimux();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void HorticulousSlimux::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Horticulous Slimux", factoryMethod);
    }
}

HorticulousSlimux::HorticulousSlimux() :
    NurgleBase("Horticulous Slimux", 5, WOUNDS, 10, 3, false),
    m_shears(Weapon::Type::Melee, "Lopping Shears", 1, 3, 3, 3, -1, RAND_D3),
    m_jaws(Weapon::Type::Melee, "Mulch's Slime-encrusted Jaws", 1, RAND_D3, 3, 3, -2, 2)
{
    m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, HORTICULOUS_SLIMUX};
    m_weapons = {&m_shears, &m_jaws};
}

bool HorticulousSlimux::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_shears);
    model->addMeleeWeapon(&m_jaws);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Nurgle
