/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ExaltedHeroOfChaos.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ExaltedHeroOfChaos::Create,
    SlavesToDarknessBase::ValueToString,
    SlavesToDarknessBase::EnumStringToInt,
    ExaltedHeroOfChaos::ComputePoints,
    {
        {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
        {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
    },
    CHAOS,
    { SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE }
};

bool ExaltedHeroOfChaos::s_registered = false;

Unit *ExaltedHeroOfChaos::Create(const ParameterList &parameters)
{
    auto unit = new ExaltedHeroOfChaos();

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    auto mark = (MarkOfChaos)GetEnumParam("Mark of Chaos", parameters, Undivided);
    unit->setMarkOfChaos(mark);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ExaltedHeroOfChaos::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Exalted Hero of Chaos", factoryMethod);
    }
}

ExaltedHeroOfChaos::ExaltedHeroOfChaos() :
    SlavesToDarknessBase("Exalted Hero of Chaos", 5, WOUNDS, 8, 4, false),
    m_blades(Weapon::Type::Melee, "Rune-etched Blades", 1, RAND_D6, 3, 3, -1, 1)
{
    m_keywords = { CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, EXALTED_HERO_OF_CHAOS };
    m_weapons = {&m_blades};
}

bool ExaltedHeroOfChaos::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_blades);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds ExaltedHeroOfChaos::applyWoundSave(const Wounds &wounds)
{
    // Dark Blessings
    Dice::RollResult mortalSaves;
    Dice::rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.mortal -= mortalSaves.rollsGE(5);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

int ExaltedHeroOfChaos::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Glory-hungry Bladesman
    if (target->hasKeyword(HERO) || target->hasKeyword(MONSTER)) mod++;

    return mod;
}

} //namespace SlavesToDarkness
