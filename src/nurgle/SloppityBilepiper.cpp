/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/SloppityBilepiper.h"

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    SloppityBilepiperHeraldOfNurgle::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    SloppityBilepiperHeraldOfNurgle::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

bool SloppityBilepiperHeraldOfNurgle::s_registered = false;

Unit* SloppityBilepiperHeraldOfNurgle::Create(const ParameterList &parameters)
{
    auto unit = new SloppityBilepiperHeraldOfNurgle();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SloppityBilepiperHeraldOfNurgle::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sloppity Bilepiper, Herald of Nurgle", factoryMethod);
    }
}

SloppityBilepiperHeraldOfNurgle::SloppityBilepiperHeraldOfNurgle() :
    NurgleBase("Sloppity Bilepiper, Herald of Nurgle", 4, WOUNDS, 10, 4, false),
    m_marotter(Weapon::Type::Melee, "Marotter", 1, 4, 4, 3, -1, 2)
{
    m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, HERO, SLOPPITY_BILEPIPER, HERALD_OF_NURGLE};
    m_weapons = {&m_marotter};
}

bool SloppityBilepiperHeraldOfNurgle::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_marotter);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Nurgle
