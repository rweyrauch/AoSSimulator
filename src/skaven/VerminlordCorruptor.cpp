/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordCorruptor.h>
#include <UnitFactory.h>

namespace Skaven
{
bool VerminlordCorruptor::s_registered = false;

Unit *VerminlordCorruptor::Create(const ParameterList &parameters)
{
    auto unit = new VerminlordCorruptor();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VerminlordCorruptor::Init()
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

        s_registered = UnitFactory::Register("Verminlord Corruptor", *factoryMethod);
    }
}

VerminlordCorruptor::VerminlordCorruptor() :
    Skaventide("Verminlord Corruptor", 12, WOUNDS, 10, 4, false),
    m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
    m_plaguereapers(Weapon::Type::Melee, "Plaguereapers", 1, 10, 3, 3, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, NURGLE, CLANS_PESTILENS, MONSTER, HERO,
        WIZARD, VERMINLORD_CORRUPTOR};
    m_weapons = {&m_tails, &m_plaguereapers};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool VerminlordCorruptor::configure()
{
    return false;
}
} //namespace Skaven
