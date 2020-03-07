/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkStarseer.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SkinkStarseer::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    SkinkStarseer::ComputePoints,
    {},
    ORDER,
    { SERAPHON }
};

bool SkinkStarseer::s_registered = false;

SkinkStarseer::SkinkStarseer() :
    SeraphonBase("Skink Starseer", 5, WOUNDS, 6, 5, true),
    m_astralBolt(Weapon::Type::Missile, "Astral Bolt", 18, 2, 3, 3, -1, RAND_D3),
    m_staff(Weapon::Type::Melee, "Astromancer's Staff", 2, 2, 4, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, SERAPHON, SKINK, HERO, WIZARD, STARSEER};
    m_weapons = {&m_astralBolt, &m_staff};
}

bool SkinkStarseer::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_astralBolt);
    model->addMeleeWeapon(&m_staff);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SkinkStarseer::Create(const ParameterList &parameters)
{
    auto unit = new SkinkStarseer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SkinkStarseer::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skink Starseer", factoryMethod);
    }
}

} //namespace Seraphon