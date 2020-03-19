/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Furies.h"

namespace SlavesToDarkness
{

bool Furies::s_registered = false;

Unit *Furies::Create(const ParameterList &parameters)
{
    auto unit = new Furies();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Furies::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Furies::Create,
            SlavesToDarknessBase::ValueToString,
            SlavesToDarknessBase::EnumStringToInt,
            Furies::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", Furies::MIN_UNIT_SIZE, Furies::MIN_UNIT_SIZE,
                    Furies::MAX_UNIT_SIZE, Furies::MIN_UNIT_SIZE
                },
                {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
            },
            CHAOS,
            { SLAVES_TO_DARKNESS }
        };
        s_registered = UnitFactory::Register("Furies", factoryMethod);
    }
}

bool Furies::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_daggerAndClaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Furies::Furies() :
    SlavesToDarknessBase("Furies", 12, WOUNDS, 10, NoSave, true),
    m_daggerAndClaws(Weapon::Type::Melee, "Razor-sharp Dagger and Claws", 1, 2, 4, 3, -1, 1)
{
    m_keywords = { CHAOS, DAEMON, SLAVES_TO_DARKNESS, FURIES };
    m_weapons = {&m_daggerAndClaws};
}

int Furies::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //SlavesToDarkness