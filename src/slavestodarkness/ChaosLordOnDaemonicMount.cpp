/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosLordOnDaemonicMount.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosLordOnDaemonicMount::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool ChaosLordOnDaemonicMount::s_registered = false;

Unit *ChaosLordOnDaemonicMount::Create(const ParameterList &parameters)
{
    auto unit = new ChaosLordOnDaemonicMount();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosLordOnDaemonicMount::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Lord On Daemonic Mount", factoryMethod);
    }
}

ChaosLordOnDaemonicMount::ChaosLordOnDaemonicMount() :
    Unit("Chaos Lord On Daemonic Mount", 10, WOUNDS, 8, 4, false),
    m_hammer(Weapon::Type::Melee, "Cursed Warhammer", 1, 4, 3, 3, -1, 2),
    m_hooves(Weapon::Type::Melee, "Mighty Hooves", 1, 3, 4, 3, 0, 1)
{
    m_keywords = { CHAOS, DAEMON, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, CHAOS_LORD };
}

bool ChaosLordOnDaemonicMount::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_hammer);
    model.addMeleeWeapon(&m_hooves);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ChaosLordOnDaemonicMount::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_hammer);
    visitor(m_hooves);
}

Wounds ChaosLordOnDaemonicMount::applyWoundSave(const Wounds &wounds)
{
    auto savedWounds = Unit::applyWoundSave(wounds);
    Dice dice;
    Dice::RollResult result;
    // Chaos Runeshield
    dice.rollD6(savedWounds.mortal, result);
    savedWounds.mortal -= result.rollsGE(5);
    return savedWounds;
}

Wounds ChaosLordOnDaemonicMount::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    // Cursed Warhammer
    if ((hitRoll == 6) && (weapon->name() == m_hooves.name()))
    {
        damage.mortal = 2;
        damage.normal = 0;
    }
    return damage;
}

} //namespace SlavesToDarkness
