/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/RatSwarms.h>
#include <UnitFactory.h>

namespace Skaven
{
bool RatSwarms::s_registered = false;

Unit *RatSwarms::Create(const ParameterList &parameters)
{
    auto unit = new RatSwarms();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int RatSwarms::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

void RatSwarms::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,MAX_UNIT_SIZE, MIN_UNIT_SIZE},
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Rat Swarms", factoryMethod);
    }
}

RatSwarms::RatSwarms() :
    Skaventide("Rat Swarms", 6, WOUNDS, 10, NoSave, false),
    m_teeth(Weapon::Type::Melee, "Gnawing Teeth", 1, 5, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVENTIDE, CLANS_MOULDER, PACK, RAT_SWARMS};
    m_weapons = {&m_teeth};
}

bool RatSwarms::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_teeth);
        addModel(model);
    }
    m_points = ComputePoints(numModels);

    return true;
}
} //namespace Skaven
