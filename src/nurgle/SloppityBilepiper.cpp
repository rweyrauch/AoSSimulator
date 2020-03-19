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

    s_globalBraveryMod.connect(this, &SloppityBilepiperHeraldOfNurgle::diseaseOfMirthBraveryMod, &m_diseaseOfMirthSlot);
    s_globalChargeReroll.connect(this, &SloppityBilepiperHeraldOfNurgle::jollyGutpipesChargeReroll, &m_jollyGutpipesSlot);
}

SloppityBilepiperHeraldOfNurgle::~SloppityBilepiperHeraldOfNurgle()
{
    m_diseaseOfMirthSlot.disconnect();
    m_jollyGutpipesSlot.disconnect();
}

bool SloppityBilepiperHeraldOfNurgle::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_marotter);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds SloppityBilepiperHeraldOfNurgle::applyWoundSave(const Wounds &wounds)
{
    // Disgustingly Resilient
    Dice::RollResult woundSaves, mortalSaves;
    Dice::rollD6(wounds.normal, woundSaves);
    Dice::rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(5);
    totalWounds.mortal -= mortalSaves.rollsGE(5);

    return totalWounds.clamp();
}

int SloppityBilepiperHeraldOfNurgle::diseaseOfMirthBraveryMod(const Unit *unit)
{
    if (isFriendly(unit))
    {
        if ((unit->hasKeyword(NURGLE) && unit->hasKeyword(DAEMON)) && (distanceTo(unit) <= 7.0f)) return 1;
    }
    else if (distanceTo(unit) <= 7.0f)
    {
        return -1;
    }

    return 0;
}

Rerolls SloppityBilepiperHeraldOfNurgle::jollyGutpipesChargeReroll(const Unit *unit)
{
    if ((unit->hasKeyword(NURGLINGS) || unit->hasKeyword(GREAT_UNCLEAN_ONE)) && (distanceTo(unit) <= 7.0f)) return RerollOnes;

    return NoRerolls;
}

} // namespace Nurgle