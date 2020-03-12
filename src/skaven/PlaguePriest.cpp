/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlaguePriest.h>
#include <UnitFactory.h>

namespace Skaven
{
bool PlaguePriest::s_registered = false;

Unit *PlaguePriest::Create(const ParameterList &parameters)
{
    auto unit = new PlaguePriest();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PlaguePriest::Init()
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

        s_registered = UnitFactory::Register("Plague Priest", *factoryMethod);
    }
}

PlaguePriest::PlaguePriest() :
    Skaventide("Plague Priest", 6, WOUNDS, 6, 5, false),
    m_staff(Weapon::Type::Melee, "Warpstone-tipped Staff", 2, 1, 4, 3, -1, RAND_D3),
    m_censer(Weapon::Type::Melee, "Plague Censer", 2, 2, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, HERO, PRIEST, PLAGUE_PRIEST};
    m_weapons = {&m_staff, &m_censer};
}

bool PlaguePriest::configure()
{
    return false;
}
} //namespace Skaven
