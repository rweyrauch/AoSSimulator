/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/LordOfAfflictions.h"

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    LordOfAfflictions::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    LordOfAfflictions::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

bool LordOfAfflictions::s_registered = false;

Unit* LordOfAfflictions::Create(const ParameterList &parameters)
{
    auto unit = new LordOfAfflictions();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordOfAfflictions::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord of Afflictions", factoryMethod);
    }
}

LordOfAfflictions::LordOfAfflictions() :
    NurgleBase("Lord of Afflictions", 8, WOUNDS, 10, 4, true),
    m_festerspike(Weapon::Type::Melee, "Festerspike", 2, 3, 3, 3, -1, RAND_D3),
    m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
    m_sting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3),
    m_tocsin(Weapon::Type::Melee, "Dolorous Tocsin", 1, 1, 4, 3, -2, 2)
{
    m_keywords = {CHAOS, MORTAL, DAEMON, NURGLE, ROTBRINGER, HERO, LORD_OF_AFFLICTIONS};
    m_weapons = {&m_festerspike, &m_mouthparts, &m_sting, &m_tocsin};
}

bool LordOfAfflictions::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_festerspike);
    model->addMeleeWeapon(&m_mouthparts);
    model->addMeleeWeapon(&m_sting);
    model->addMeleeWeapon(&m_tocsin);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Nurgle
