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
    int numModels = 0;

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
    return 0;
}

void RatSwarms::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Rat Swarms", *factoryMethod);
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
    return false;
}
} //namespace Skaven
