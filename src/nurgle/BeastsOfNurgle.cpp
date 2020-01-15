/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/BeastsOfNurgle.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    BeastsOfNurgle::Create,
    nullptr,
    nullptr,
    BeastsOfNurgle::ComputePoints,
    {
        {
            ParamType::Integer, "Models", BeastsOfNurgle::MIN_UNIT_SIZE, BeastsOfNurgle::MIN_UNIT_SIZE,
            BeastsOfNurgle::MAX_UNIT_SIZE, BeastsOfNurgle::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    { NURGLE }
};

bool BeastsOfNurgle::s_registered = false;

BeastsOfNurgle::BeastsOfNurgle() :
    Unit("Beasts of Nurgle", 5, WOUNDS, 10, 5, false),
    m_clawsAndTentacles(Weapon::Type::Melee, "Claws and Tentacles", 1, RAND_D6, 4, 3, 0, 1),
    m_slobberingTongue(Weapon::Type::Melee, "Slobbering Tongue", 2, 1, 3, 3, 0, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, NURGLE, BEASTS_OF_NURGLE};
    m_weapons = {&m_clawsAndTentacles, &m_slobberingTongue};
}

bool BeastsOfNurgle::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_clawsAndTentacles);
        model->addMeleeWeapon(&m_slobberingTongue);
        addModel(model);
    }

    // Attention Seekers
    m_runAndCharge = true;
    m_retreatAndCharge = true;

    m_points = ComputePoints(numModels);

    return true;
}

Unit *BeastsOfNurgle::Create(const ParameterList &parameters)
{
    auto unit = new BeastsOfNurgle();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BeastsOfNurgle::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Beasts of Nurgle", factoryMethod);
    }
}

Wounds BeastsOfNurgle::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    // Disgustingly Resilient
    Dice::RollResult woundSaves, mortalSaves;
    dice.rollD6(wounds.normal, woundSaves);
    dice.rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(5);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(5);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

Wounds BeastsOfNurgle::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Locus of Virulence
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 7.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(DAEMON) && ip->hasKeyword(NURGLE) && ip->hasKeyword(HERO))
        {
            return {weapon->damage() + 1, 0};
        }
    }

    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int BeastsOfNurgle::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Nurgle

