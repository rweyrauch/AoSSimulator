/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/ArchWarlock.h>
#include <UnitFactory.h>

namespace Skaven
{
bool ArchWarlock::s_registered = false;

Unit *ArchWarlock::Create(const ParameterList &parameters)
{
    auto unit = new ArchWarlock();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ArchWarlock::Init()
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

        s_registered = UnitFactory::Register("Arch-Warlock", *factoryMethod);
    }
}

ArchWarlock::ArchWarlock() :
    Skaventide("Arch-Warlock", 6, WOUNDS, 6, 3, false),
    m_halberd(Weapon::Type::Melee, "Stormcage Halberd", 2, 1, 3, 3, -2, RAND_D3),
    m_claw(Weapon::Type::Melee, "Piston Claw", 1, 1, 4, 3, -2, 3)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, HERO, WIZARD, WARLOCK_ENGINEER, ARCH_WARLOCK};
    m_weapons = {&m_halberd, &m_claw};

    m_totalSpells = 2;
    m_totalUnbinds = 1;
}

bool ArchWarlock::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_halberd);
    model->addMeleeWeapon(&m_claw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //namespace Skaven
