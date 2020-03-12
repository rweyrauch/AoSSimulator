/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordWarbringer.h>
#include <UnitFactory.h>

namespace Skaven
{
bool VerminlordWarbringer::s_registered = false;

Unit *VerminlordWarbringer::Create(const ParameterList &parameters)
{
    auto unit = new VerminlordWarbringer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VerminlordWarbringer::Init()
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

        s_registered = UnitFactory::Register("Verminlord Warbringer", *factoryMethod);
    }
}

VerminlordWarbringer::VerminlordWarbringer() :
    Skaventide("Verminlord Warbringer", 12, WOUNDS, 10, 4, false),
    m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
    m_glaive(Weapon::Type::Melee, "Doom Glaive", 3, 6, 3, 3, -1, RAND_D3),
    m_fist(Weapon::Type::Melee, "Spike-fist", 1, 1, 3, 2, -2, 2)
{
    m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, CLANS_VERMINUS, MONSTER, HERO, WIZARD,
        VERMINLORD_WARBRINGER};
    m_weapons = {&m_tails, &m_glaive, &m_fist};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool VerminlordWarbringer::configure()
{
    return false;
}
} //namespace Skaven
