/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarplockJezzails.h>
#include <UnitFactory.h>

namespace Skaven
{
bool WarplockJezzails::s_registered = false;

Unit *WarplockJezzails::Create(const ParameterList &parameters)
{
    auto unit = new WarplockJezzails();
    int numModels = 0;

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int WarplockJezzails::ComputePoints(int numModels)
{
    return 0;
}

void WarplockJezzails::Init()
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

        s_registered = UnitFactory::Register("Warplock Jezzails", *factoryMethod);
    }
}

WarplockJezzails::WarplockJezzails() :
    Skaventide("Warplock Jezzails", 6, WOUNDS, 4, 6, false),
    m_jezzail(Weapon::Type::Missile, "Warplock Jezzail", 30, 1, 4, 3, -2, 2),
    m_knives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WARPLOCK_JEZZAILS};
    m_weapons = {&m_jezzail, &m_knives};
}

bool WarplockJezzails::configure(int numModels)
{
    return false;
}
} //namespace Skaven
