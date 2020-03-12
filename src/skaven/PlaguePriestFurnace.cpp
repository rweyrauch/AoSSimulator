/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlaguePriestFurnace.h>
#include <UnitFactory.h>

namespace Skaven
{
bool PlaguePriestOnPlagueFurnace::s_registered = false;

Unit *PlaguePriestOnPlagueFurnace::Create(const ParameterList &parameters)
{
    auto unit = new PlaguePriestOnPlagueFurnace();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PlaguePriestOnPlagueFurnace::Init()
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

        s_registered = UnitFactory::Register("Plague Priest on Plague Furnace", *factoryMethod);
    }
}

PlaguePriestOnPlagueFurnace::PlaguePriestOnPlagueFurnace() :
    Skaventide("Plague Priest on Plague Furnace", 6, WOUNDS, 10, 4, false),
    m_censer(Weapon::Type::Melee, "Great Plague Censer", 3, 0, 0, 0, 0, 0),
    m_staff(Weapon::Type::Melee, "Warpstone-tipped Staff", 2, 1, 4, 3, -1, RAND_D3),
    m_blades(Weapon::Type::Melee, "Foetid Blades", 1, 6, 4, 4, 0, 1),
    m_spikes(Weapon::Type::Melee, "Rusty Spikes", 1, RAND_D6, 2, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, WAR_MACHINE, HERO, PRIEST,
    PLAGUE_FURNACE, PLAGUE_PRIEST};
    m_weapons = {&m_censer, &m_staff, &m_blades, &m_spikes};
}

bool PlaguePriestOnPlagueFurnace::configure()
{
    return false;
}
} //namespace Skaven
