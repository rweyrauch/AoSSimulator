/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/SpoilpoxScrivener.h"

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    SpoilpoxScrivenerHeraldOfNurgle::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    SpoilpoxScrivenerHeraldOfNurgle::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

bool SpoilpoxScrivenerHeraldOfNurgle::s_registered = false;

Unit* SpoilpoxScrivenerHeraldOfNurgle::Create(const ParameterList &parameters)
{
    auto unit = new SpoilpoxScrivenerHeraldOfNurgle();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SpoilpoxScrivenerHeraldOfNurgle::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Spoilpox Scrivener, Herald of Nurgle", factoryMethod);
    }
}

SpoilpoxScrivenerHeraldOfNurgle::SpoilpoxScrivenerHeraldOfNurgle() :
    NurgleBase("Spoilpox Scrivener, Herald of Nurgle", 4, WOUNDS, 10, 4, false),
    m_sneeze(Weapon::Type::Missile, "Disgusting Sneeze", 6, RAND_D6, 3, 4, 0, 1),
    m_maw(Weapon::Type::Melee, "Distended Maw", 2, 2, 3, 4, -1, 2)
{
    m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, SPOILPOX_SCRIVENER, HERALD_OF_NURGLE};
    m_weapons = {&m_sneeze, &m_maw};
}

bool SpoilpoxScrivenerHeraldOfNurgle::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_sneeze);
    model->addMeleeWeapon(&m_maw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Nurgle
