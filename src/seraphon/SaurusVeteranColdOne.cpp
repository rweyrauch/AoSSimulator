/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusVeteranColdOne.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SaurusScarVeteranOnColdOne::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    SaurusScarVeteranOnColdOne::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

bool SaurusScarVeteranOnColdOne::s_registered = false;

SaurusScarVeteranOnColdOne::SaurusScarVeteranOnColdOne() :
    SeraphonBase("Saurus Scar-Veteran on Cold One", 8, WOUNDS, 8, 4, false),
    m_warpick(Weapon::Type::Melee, "Celestite Warpick", 1, 3, 3, 3, -1, 1),
    m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1),
    m_coldOneJaws(Weapon::Type::Melee, "Snapping Jaws", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SAURUS, HERO, COLD_ONE, SCAR_VETERAN};
    m_weapons = {&m_warpick, &m_jaws, &m_coldOneJaws};
}

bool SaurusScarVeteranOnColdOne::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_warpick);
    model->addMeleeWeapon(&m_jaws);
    model->addMeleeWeapon(&m_coldOneJaws);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SaurusScarVeteranOnColdOne::Create(const ParameterList &parameters)
{
    auto unit = new SaurusScarVeteranOnColdOne();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SaurusScarVeteranOnColdOne::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Saurus Scar-Veteran on Cold One", factoryMethod);
    }
}

} //namespace Seraphon
