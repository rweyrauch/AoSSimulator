/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/LordOfBlights.h"

namespace Nurgle
{

bool LordOfBlights::s_registered = false;

Unit* LordOfBlights::Create(const ParameterList &parameters)
{
    auto unit = new LordOfBlights();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordOfBlights::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            LordOfBlights::Create,
            NurgleBase::ValueToString,
            NurgleBase::EnumStringToInt,
            LordOfBlights::ComputePoints,
            {
            },
            CHAOS,
            { NURGLE }
        };
        s_registered = UnitFactory::Register("Lord of Blights", factoryMethod);
    }
}

LordOfBlights::LordOfBlights() :
    NurgleBase("Lord of Blights", 4, WOUNDS, 8, 4, false),
    m_ripenedDeathsHead(Weapon::Type::Missile, "Thrice-ripened Death's Head", 14, 1, 3, 3, -3, RAND_D3),
    m_bountyDeathsHead(Weapon::Type::Missile, "Munificent Bounty Death's Head", 14, 1, 4, 3, 0, 1),
    m_hammer(Weapon::Type::Melee, "Bubotic Hammer", 1, 3, 3, 3, -1, 2)
{
    m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, LORD_OF_BLIGHTS};
    m_weapons = {&m_ripenedDeathsHead, &m_bountyDeathsHead, &m_hammer};
}

bool LordOfBlights::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_ripenedDeathsHead);
    model->addMissileWeapon(&m_bountyDeathsHead);
    model->addMeleeWeapon(&m_hammer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Rerolls LordOfBlights::toSaveRerolls(const Weapon *weapon) const
{
    // Vermid Shield
    if (!weapon->isMissile()) return RerollOnes;
    return Unit::toSaveRerolls(weapon);
}

} // namespace Nurgle
