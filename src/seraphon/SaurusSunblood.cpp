/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusSunblood.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SaurusSunblood::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    SaurusSunblood::ComputePoints,
    {},
    ORDER,
    { SERAPHON }
};

bool SaurusSunblood::s_registered = false;

SaurusSunblood::SaurusSunblood() :
    SeraphonBase("Saurus Sunblood", 5, WOUNDS, 10, 4, false),
    m_celestiteWarmace(Weapon::Type::Melee, "Celestite War-mace", 1, 5, 3, 3, -1, 1),
    m_jawsAndShield(Weapon::Type::Melee, "Fearsome Jaws and Aeon Shield", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SAURUS, HERO, SAURUS_SUNBLOOD};
    m_weapons = {&m_celestiteWarmace, &m_jawsAndShield};
}

bool SaurusSunblood::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_celestiteWarmace);
    model->addMeleeWeapon(&m_jawsAndShield);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SaurusSunblood::Create(const ParameterList &parameters)
{
    auto unit = new SaurusSunblood();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SaurusSunblood::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Saurus Sunblood", factoryMethod);
    }
}

} //namespace Seraphon
