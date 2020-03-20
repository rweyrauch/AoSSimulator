/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/DarkoathWarqueen.h"

namespace SlavesToDarkness
{
static const int BASESIZE = 32;
static const int WOUNDS = 6;
static const int POINTS_PER_UNIT = 90;

bool DarkoathWarqueen::s_registered = false;

Unit *DarkoathWarqueen::Create(const ParameterList &parameters)
{
    auto unit = new DarkoathWarqueen();

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DarkoathWarqueen::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            DarkoathWarqueen::Create,
            SlavesToDarknessBase::ValueToString,
            SlavesToDarknessBase::EnumStringToInt,
            DarkoathWarqueen::ComputePoints,
            {
                {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
            },
            CHAOS,
            { SLAVES_TO_DARKNESS }
        };
        s_registered = UnitFactory::Register("Darkoath Warqueen", factoryMethod);
    }
}

DarkoathWarqueen::DarkoathWarqueen() :
    SlavesToDarknessBase("Darkoath Warqueen", 6, WOUNDS, 8, 5, false),
    m_axe(Weapon::Type::Melee, "Rune-etched Axe", 1, 6, 3, 3, -1, 1)
{
    m_keywords = { CHAOS, MORTAL, SLAVES_TO_DARKNESS, EYE_OF_THE_GODS, HERO, DARKOATH_WARQUEEN };
    m_weapons = {&m_axe};
}

bool DarkoathWarqueen::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_axe);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int DarkoathWarqueen::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace SlavesToDarkness
