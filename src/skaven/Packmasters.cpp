/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Packmasters.h>
#include <UnitFactory.h>

namespace Skaven
{
bool Packmasters::s_registered = false;

Unit *Packmasters::Create(const ParameterList &parameters)
{
    auto unit = new Packmasters();
    int numModels = 0;

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int Packmasters::ComputePoints(int numModels)
{
    return 0;
}

void Packmasters::Init()
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

        s_registered = UnitFactory::Register("Packmasters", *factoryMethod);
    }
}

Packmasters::Packmasters() :
    Skaventide("Packmasters", 6, WOUNDS, 5, 6, false),
    m_whip(Weapon::Type::Melee, "Herding Whip", 3, 1, 4, 4, 0, 1),
    m_blade(Weapon::Type::Melee, "Rusty Blade", 1, 2, 4, 4, 0, 1),
    m_catcher(Weapon::Type::Melee, "Things-catcher", 2, 1, 4, 4, -1, 2)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, PACKMASTERS};
    m_weapons = {&m_whip, &m_blade, &m_catcher};
}

bool Packmasters::configure(int numModels)
{
    return false;
}
} //namespace Skaven
