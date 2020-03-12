/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarlockBombardier.h>
#include <UnitFactory.h>

namespace Skaven
{
bool WarlockBombardier::s_registered = false;

Unit *WarlockBombardier::Create(const ParameterList &parameters)
{
    auto unit = new WarlockBombardier();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WarlockBombardier::Init()
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

        s_registered = UnitFactory::Register("Warlock Bombardier", *factoryMethod);
    }
}

WarlockBombardier::WarlockBombardier() :
    Skaventide("Warlock Bombardier", 6, WOUNDS, 5, 5, false),
    m_doomrocket(Weapon::Type::Missile, "Doomrocket", 18, 1, 4, 3, -1, RAND_D6),
    m_pole(Weapon::Type::Melee, "Firing Pole", 1, 1, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, HERO, WIZARD, WARLOCK_ENGINEER,
        WARLOCK_BOMBARDIER};
    m_weapons = {&m_doomrocket, &m_pole};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool WarlockBombardier::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_doomrocket);
    model->addMeleeWeapon(&m_pole);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}
} //namespace Skaven
