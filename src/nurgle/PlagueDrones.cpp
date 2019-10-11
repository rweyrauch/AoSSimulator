/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/PlagueDrones.h>
#include <UnitFactory.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    PlagueDrones::Create,
    PlagueDrones::ValueToString,
    PlagueDrones::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", PlagueDrones::MIN_UNIT_SIZE, PlagueDrones::MIN_UNIT_SIZE,
            PlagueDrones::MAX_UNIT_SIZE, PlagueDrones::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", PlagueDrones::PrehensileProboscis, PlagueDrones::PrehensileProboscis, PlagueDrones::FoulMouthparts},
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Bell Tollers", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    NURGLE
};

bool PlagueDrones::s_registered = false;

PlagueDrones::PlagueDrones() :
    Unit("Plague Drones", 8, WOUNDS, 10, 5, true),
    m_plaguesword(Weapon::Type::Melee, "Plaguesword", 1, 1, 4, 3, 0, 1),
    m_plagueswordPlaguebringer(Weapon::Type::Melee, "Plaguesword", 1, 2, 4, 3, 0, 1),
    m_deathsHead(Weapon::Type::Missile, "Death's Head", 14, 1, 4, 3, 0, 1),
    m_proboscis(Weapon::Type::Melee, "Prehensile Proboscis", 1, 3, 3, 4, 0, 1),
    m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
    m_venemousSting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, PLAGUEBEARER, NURGLE, PLAGUE_DRONES};
}

bool PlagueDrones::configure(int numModels, WeaponOption weapons, bool iconBearer, bool bellTollers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_bellTollers = bellTollers;
    m_weapon = weapons;

    // Add the Plaguebringer
    Model leader(BASESIZE, WOUNDS);
    leader.addMissileWeapon(&m_deathsHead);
    leader.addMeleeWeapon(&m_plagueswordPlaguebringer);
    if (weapons == PrehensileProboscis)
    {
        leader.addMeleeWeapon(&m_proboscis);
    }
    else if (weapons == FoulMouthparts)
    {
        leader.addMeleeWeapon(&m_mouthparts);
    }
    leader.addMeleeWeapon(&m_venemousSting);
    addModel(leader);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_deathsHead);
        model.addMeleeWeapon(&m_plaguesword);
        if (weapons == PrehensileProboscis)
        {
            model.addMeleeWeapon(&m_proboscis);
        }
        else if (weapons == FoulMouthparts)
        {
            model.addMeleeWeapon(&m_mouthparts);
        }
        model.addMeleeWeapon(&m_venemousSting);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void PlagueDrones::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_plaguesword);
    visitor(&m_plagueswordPlaguebringer);
    visitor(&m_deathsHead);
    visitor(&m_proboscis);
    visitor(&m_mouthparts);
    visitor(&m_venemousSting);
}

Unit *PlagueDrones::Create(const ParameterList &parameters)
{
    auto unit = new PlagueDrones();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, PrehensileProboscis);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool bells = GetBoolParam("Bell Tollers", parameters, false);

    bool ok = unit->configure(numModels, weapons, iconBearer, bells);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PlagueDrones::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Plague Drones", factoryMethod);
    }
}

Wounds PlagueDrones::applyWoundSave(const Wounds &wounds)
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

void PlagueDrones::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const
{
    Unit::computeBattleshockEffect(roll, numFled, numAdded);
    if (m_iconBearer)
    {
        // Icon Bearer
        if (roll == 1)
        {
            Dice dice;
            numAdded = dice.rollD6();
        }
    }
}

void PlagueDrones::restoreModels(int numModels)
{
    // Icon Bearer
    Model model(BASESIZE, WOUNDS);
    model.addMissileWeapon(&m_deathsHead);
    model.addMeleeWeapon(&m_plaguesword);
    if (m_weapon == PrehensileProboscis)
    {
        model.addMeleeWeapon(&m_proboscis);
    }
    else if (m_weapon == FoulMouthparts)
    {
        model.addMeleeWeapon(&m_mouthparts);
    }
    model.addMeleeWeapon(&m_venemousSting);
    for (auto i = 0; i < numModels; i++)
    {
        addModel(model);
    }
}

std::string PlagueDrones::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == PrehensileProboscis) { return "Prehensile Proboscis"; }
        else if (parameter.m_intValue == FoulMouthparts) { return "Foul Mouthparts"; }
    }
    return ParameterValueToString(parameter);
}

int PlagueDrones::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Prehensile Proboscis") { return PrehensileProboscis; }
    else if (enumString == "Foul Mouthparts") { return FoulMouthparts; }
    return 0;
}

} // namespace Nurgle

