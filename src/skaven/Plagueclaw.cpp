/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Plagueclaw.h>
#include <UnitFactory.h>

namespace Skaven
{
bool Plagueclaw::s_registered = false;

Unit *Plagueclaw::Create(const ParameterList &parameters)
{
    auto unit = new Plagueclaw();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Plagueclaw::Init()
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

        s_registered = UnitFactory::Register("Plagueclaw", *factoryMethod);
    }
}

Plagueclaw::Plagueclaw() :
    Skaventide("Plagueclaw", 3, WOUNDS, 4, 5, false),
    m_catapult(Weapon::Type::Missile, "Plagueclaw Catapult", 31, 1, 3, 3, -2, RAND_D6),
    m_knives(Weapon::Type::Melee, "Rusty Knives", 1, RAND_D6, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, WAR_MACHINE, PLAGUECLAW};
    m_weapons = {&m_catapult, &m_knives};
}

bool Plagueclaw::configure()
{
    return false;
}
} //namespace Skaven
