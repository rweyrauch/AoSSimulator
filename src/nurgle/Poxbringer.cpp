/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Poxbringer.h"

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    PoxbringerHeraldOfNurgle::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    PoxbringerHeraldOfNurgle::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

bool PoxbringerHeraldOfNurgle::s_registered = false;

Unit* PoxbringerHeraldOfNurgle::Create(const ParameterList &parameters)
{
    auto unit = new PoxbringerHeraldOfNurgle();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PoxbringerHeraldOfNurgle::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Poxbringer, Herald of Nurgle", factoryMethod);
    }
}

PoxbringerHeraldOfNurgle::PoxbringerHeraldOfNurgle() :
    NurgleBase("Poxbringer, Herald of Nurgle", 4, WOUNDS, 10, 4, false),
    m_balesword(Weapon::Type::Melee, "Balesword", 1, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, POXBRINGER, HERALD_OF_NURGLE};
    m_weapons = {&m_balesword};
}

bool PoxbringerHeraldOfNurgle::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_balesword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Nurgle
