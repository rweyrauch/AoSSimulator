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
    SeraphonBase("Saurus Scar-Veteran on Cold One", 10, WOUNDS, 10, 4, false),
    m_warpick(Weapon::Type::Melee, "Celestite War-pick", 1, 3, 3, 3, -1, 1),
    m_jawsAndShield(Weapon::Type::Melee, "Fearsome Jaws and Stardrake Shield", 1, 1, 4, 3, 0, 1),
    m_bite(Weapon::Type::Melee, "Cold One's Vicious Bite", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SAURUS, HERO, SAURUS_SCAR_VETERAN_ON_COLD_ONE};
    m_weapons = {&m_warpick, &m_jawsAndShield, &m_bite};
}

bool SaurusScarVeteranOnColdOne::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_warpick);
    model->addMeleeWeapon(&m_bite);
    model->addMeleeWeapon(&m_jawsAndShield);
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
