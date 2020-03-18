/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/HarbingerOfDecay.h"

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    HarbingerOfDecay::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    HarbingerOfDecay::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

bool HarbingerOfDecay::s_registered = false;

Unit* HarbingerOfDecay::Create(const ParameterList &parameters)
{
    auto unit = new HarbingerOfDecay();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void HarbingerOfDecay::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Harbinger of Decay", factoryMethod);
    }
}

HarbingerOfDecay::HarbingerOfDecay() :
    NurgleBase("Harbinger of Decay", 4, WOUNDS, 8, 4, false),
    m_scythe(Weapon::Type::Melee, "Plague Scythe", 1, 3, 3, 3, -1, RAND_D3),
    m_bite(Weapon::Type::Melee, "Daemonic Mount's Flyblown Bite", 1, RAND_D6, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, DAEMON, NURGLE, ROTBRINGER, HERO, HARBINGER_OF_DECAY};
    m_weapons = {&m_scythe, &m_bite};
}

bool HarbingerOfDecay::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_scythe);
    model->addMeleeWeapon(&m_bite);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Nurgle
