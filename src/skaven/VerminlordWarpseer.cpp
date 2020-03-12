/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordWarpseer.h>
#include <UnitFactory.h>

namespace Skaven
{
bool VerminlordWarpseer::s_registered = false;

Unit *VerminlordWarpseer::Create(const ParameterList &parameters)
{
    auto unit = new VerminlordWarpseer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VerminlordWarpseer::Init()
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

        s_registered = UnitFactory::Register("Verminlord Warpseer", *factoryMethod);
    }
}

VerminlordWarpseer::VerminlordWarpseer() :
    Skaventide("Verminlord Warpseer", 12, WOUNDS, 10, 4, false),
    m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
    m_glaive(Weapon::Type::Melee, "Doom Glaive", 3, 6, 3, 2, -1, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, MASTERCLAN, MONSTER, HERO, WIZARD,
                  VERMINLORD_WARPSEER};
    m_weapons = {&m_tails, &m_glaive};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool VerminlordWarpseer::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_tails);
    model->addMeleeWeapon(&m_glaive);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}
} //namespace Skaven
