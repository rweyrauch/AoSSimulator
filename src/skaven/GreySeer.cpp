/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GreySeer.h>
#include <UnitFactory.h>

namespace Skaven
{
bool GreySeer::s_registered = false;

Unit *GreySeer::Create(const ParameterList &parameters)
{
    auto unit = new GreySeer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GreySeer::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Grey Seer", factoryMethod);
    }
}

GreySeer::GreySeer() :
    Skaventide("Grey Seer", 6, WOUNDS, 6, 5, false),
    m_staff(Weapon::Type::Melee, "Warpstone Staff", 2, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, HERO, WIZARD, GREY_SEER};
    m_weapons = {&m_staff};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool GreySeer::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_staff);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //namespace Skaven
