/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlagueCenserBearers.h>
#include <UnitFactory.h>

namespace Skaven
{
bool PlagueCenserBearers::s_registered = false;

Unit *PlagueCenserBearers::Create(const ParameterList &parameters)
{
    auto unit = new PlagueCenserBearers();
    int numModels = 0;

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int PlagueCenserBearers::ComputePoints(int numModels)
{
    return 0;
}

void PlagueCenserBearers::Init()
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

        s_registered = UnitFactory::Register("Plague Censer Bearers", *factoryMethod);
    }
}

PlagueCenserBearers::PlagueCenserBearers() :
    Skaventide("Plague Censer Bearers", 6, WOUNDS, 5, 6, false),
    m_censer(Weapon::Type::Melee, "Plague Censer", 2, 2, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, PLAGUE_CENSER_BEARERS};
    m_weapons = {&m_censer};
}

bool PlagueCenserBearers::configure(int numModels)
{
    return false;
}
} //namespace Skaven
