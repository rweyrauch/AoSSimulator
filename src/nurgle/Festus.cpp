/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Festus.h"

namespace Nurgle
{

bool FestusTheLeechlord::s_registered = false;

Unit* FestusTheLeechlord::Create(const ParameterList &parameters)
{
    auto unit = new FestusTheLeechlord();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void FestusTheLeechlord::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            FestusTheLeechlord::Create,
            NurgleBase::ValueToString,
            NurgleBase::EnumStringToInt,
            FestusTheLeechlord::ComputePoints,
            {
            },
            CHAOS,
            { NURGLE }
        };
        s_registered = UnitFactory::Register("Festus the Leechlord", factoryMethod);
    }
}

FestusTheLeechlord::FestusTheLeechlord() :
    NurgleBase("Festus the Leechlord", 4, WOUNDS, 7, 5, false),
    m_staff(Weapon::Type::Melee, "Plague Staff", 1, 2, 4, 3, 0, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, WIZARD, FESTUS_THE_LEECHLORD};
    m_weapons = {&m_staff};
}

bool FestusTheLeechlord::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_staff);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Nurgle
