/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/ThanquolBoneripper.h>
#include <UnitFactory.h>

namespace Skaven
{
bool ThanquolOnBoneripper::s_registered = false;

Unit *ThanquolOnBoneripper::Create(const ParameterList &parameters)
{
    auto unit = new ThanquolOnBoneripper();
    int numProjectors = 0;

    bool ok = unit->configure(numProjectors);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ThanquolOnBoneripper::Init()
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

        s_registered = UnitFactory::Register("Thanquol on Boneripper", *factoryMethod);
    }
}

ThanquolOnBoneripper::ThanquolOnBoneripper() :
    Skaventide("Thanquol on Boneripper", 10, WOUNDS, 7, 4, false),
    m_projectors(Weapon::Type::Missile, "Warpfire Projectors", 8, 0, 0, 0, 0, 0),
    m_staff(Weapon::Type::Melee, "Staff of the Horned Rat", 2, 2, 4, 3, -1, RAND_D3),
    m_braziers(Weapon::Type::Melee, "Warpfire Braziers", 2, 0, 3, 3, -2, 3),
    m_blows(Weapon::Type::Melee, "Crushing Blows", 2, 6, 4, 3, -1, 2)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, MONSTER, HERO, WIZARD, GREY_SEER, THANQUOL};
    m_weapons = {&m_projectors, &m_staff, &m_braziers, &m_blows};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool ThanquolOnBoneripper::configure(int numProjectors)
{
    auto model = new Model(BASESIZE, WOUNDS);

    for (auto i = 0; i < numProjectors; i++)
        model->addMissileWeapon(&m_projectors);
    for (auto i = numProjectors-1; i < 4; i++)
        model->addMeleeWeapon(&m_braziers);

    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_blows);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //namespace Skaven
