/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/TheUnmade.h"

namespace SlavesToDarkness
{

bool TheUnmade::s_registered = false;

Unit *TheUnmade::Create(const ParameterList &parameters)
{
    auto unit = new TheUnmade();
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

void TheUnmade::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            SlavesToDarknessBase::ValueToString,
            SlavesToDarknessBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
            },
            CHAOS,
            { SLAVES_TO_DARKNESS }
        };

        s_registered = UnitFactory::Register("The Unmade", factoryMethod);
    }
}

TheUnmade::TheUnmade() :
    SlavesToDarknessBase("The Unmade", 6, WOUNDS, 5, 6, false),
    m_maimingWeapons(Weapon::Type::Melee, "Maiming Weapons", 1, 1, 4, 4, 0, 1),
    m_maimingWeaponsLeader(Weapon::Type::Melee, "Maiming Weapons (Joyous One)", 1, 2, 4, 4, 0, 1),
    m_nigthmareSickles(Weapon::Type::Melee, "Nightmare Sickles", 1, 3, 4, 3, -1, 2)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, THE_UNMADE};
    m_weapons = {&m_maimingWeapons, &m_maimingWeaponsLeader, &m_nigthmareSickles};

    s_globalBraveryMod.connect(this, &TheUnmade::frozenInFear, &m_connection);
}

TheUnmade::~TheUnmade()
{
    m_connection.disconnect();
}

bool TheUnmade::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto joyousOne = new Model(BASESIZE, wounds());
    joyousOne->addMeleeWeapon(&m_maimingWeaponsLeader);
    joyousOne->setName("Joyous One");
    addModel(joyousOne);

    auto blissfulOne = new Model(BASESIZE, wounds());
    blissfulOne->addMeleeWeapon(&m_nigthmareSickles);
    blissfulOne->setName("Blissful One");
    addModel(blissfulOne);

    for (auto i = 2; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_maimingWeapons);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int TheUnmade::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int TheUnmade::frozenInFear(const Unit *unit)
{
    // Frozen in Fear
    if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 6.0f))
    {
        return -1;
    }
    return 0;
}

} //SlavesToDarkness