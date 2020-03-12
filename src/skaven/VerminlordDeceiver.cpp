/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordDeceiver.h>
#include <UnitFactory.h>

namespace Skaven
{
bool VerminlordDeceiver::s_registered = false;

Unit *VerminlordDeceiver::Create(const ParameterList &parameters)
{
    auto unit = new VerminlordDeceiver();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VerminlordDeceiver::Init()
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

        s_registered = UnitFactory::Register("Verminlord Deceiver", *factoryMethod);
    }
}

VerminlordDeceiver::VerminlordDeceiver() :
    Skaventide("Verminlord Deceiver", 12, WOUNDS, 10, 4, false),
    m_doomstar(Weapon::Type::Missile, "Doomstar", 13, 1, 3, 3, -1, RAND_D3),
    m_tails(Weapon::Type::Missile, "Prehensile Tail", 6, 4, 3, 3, -1, 1),
    m_warpstiletto(Weapon::Type::Melee, "Warpstiletto", 1, 6, 3, 2, -3, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, CLANS_ESHIN, MONSTER, HERO, WIZARD,
        VERMINLORD_DECEIVER};
    m_weapons = {&m_doomstar, &m_tails, &m_warpstiletto};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool VerminlordDeceiver::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_doomstar);
    model->addMissileWeapon(&m_tails);
    model->addMeleeWeapon(&m_warpstiletto);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //namespace Skaven
