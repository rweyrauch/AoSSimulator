/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosWarriors.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosWarriors::Create,
    ChaosWarriors::ValueToString,
    ChaosWarriors::EnumStringToInt,
    ChaosWarriors::ComputePoints,
    {
        {
            ParamType::Integer, "Models", ChaosWarriors::MIN_UNIT_SIZE, ChaosWarriors::MIN_UNIT_SIZE,
            ChaosWarriors::MAX_UNIT_SIZE, ChaosWarriors::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", ChaosWarriors::HandWeaponAndShield, ChaosWarriors::HandWeaponAndShield,
            ChaosWarriors::PairedHandWeapons, 1
        },
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
        {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
    },
    CHAOS,
    { SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE }
};

bool ChaosWarriors::s_registered = false;

Unit *ChaosWarriors::Create(const ParameterList &parameters)
{
    auto unit = new ChaosWarriors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, PairedHandWeapons);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    auto mark = (MarkOfChaos)GetEnumParam("Mark of Chaos", parameters, Undivided);
    unit->setMarkOfChaos(mark);

    bool ok = unit->configure(numModels, weapons, standardBearer, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosWarriors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Warriors", factoryMethod);
    }
}

ChaosWarriors::ChaosWarriors() :
    SlavesToDarknessBase("Chaos Warriors", 5, WOUNDS, 7, 4, false),
    m_handWeapons(Weapon::Type::Melee, "Chaos Hand Weapons", 1, 2, 3, 3, 0, 1),
    m_halberd(Weapon::Type::Melee, "Chaos Halberd", 2, 2, 3, 4, 0, 1),
    m_greatBlade(Weapon::Type::Melee, "Chaos Greatblade", 1, 2, 4, 3, -1, 1),
    m_handWeaponsChampion(Weapon::Type::Melee, "Chaos Hand Weapons", 1, 3, 3, 3, 0, 1),
    m_halberdChampion(Weapon::Type::Melee, "Chaos Halberd", 2, 3, 3, 4, 0, 1),
    m_greatBladeChampion(Weapon::Type::Melee, "Chaos Greatblade", 1, 3, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_WARRIORS};
    m_weapons = {&m_handWeapons, &m_halberd, &m_greatBlade, &m_handWeaponsChampion, &m_halberdChampion, &m_greatBladeChampion};
}

bool ChaosWarriors::configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    m_hasShields = false;
    m_pairedWeapons = false;

    auto champion = new Model(BASESIZE, WOUNDS);
    if (weapons == HandWeaponAndShield)
    {
        champion->addMeleeWeapon(&m_handWeaponsChampion);
        m_hasShields = true;
    }
    else if (weapons == HalberdAndShield)
    {
        champion->addMeleeWeapon(&m_halberdChampion);
        m_hasShields = true;
    }
    else if (weapons == GreatBlade)
    {
        champion->addMeleeWeapon(&m_greatBladeChampion);
    }
    else if (weapons == PairedHandWeapons)
    {
        champion->addMeleeWeapon(&m_handWeaponsChampion);
        m_pairedWeapons = true;
    }
    champion->setName("Aspiring Champion");
    addModel(champion);

    if (m_standardBearer)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Standard Bearer");
        if (weapons == HandWeaponAndShield)
            model->addMeleeWeapon(&m_handWeapons);
        else if (weapons == HalberdAndShield)
            model->addMeleeWeapon(&m_halberd);
        else if (weapons == GreatBlade)
            model->addMeleeWeapon(&m_greatBlade);
        else if (weapons == PairedHandWeapons)
            model->addMeleeWeapon(&m_handWeapons);
        addModel(model);
    }

    if (m_hornblower)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Hornblower");
        if (weapons == HandWeaponAndShield)
            model->addMeleeWeapon(&m_handWeapons);
        else if (weapons == HalberdAndShield)
            model->addMeleeWeapon(&m_halberd);
        else if (weapons == GreatBlade)
            model->addMeleeWeapon(&m_greatBlade);
        else if (weapons == PairedHandWeapons)
            model->addMeleeWeapon(&m_handWeapons);
        addModel(model);
    }

    for (auto i = (int)m_models.size(); i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == HandWeaponAndShield)
            model->addMeleeWeapon(&m_handWeapons);
        else if (weapons == HalberdAndShield)
            model->addMeleeWeapon(&m_halberd);
        else if (weapons == GreatBlade)
            model->addMeleeWeapon(&m_greatBlade);
        else if (weapons == PairedHandWeapons)
            model->addMeleeWeapon(&m_handWeapons);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

std::string ChaosWarriors::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == HandWeaponAndShield)
        {
            return "Hand Weapon and Shield";
        }
        else if (parameter.intValue == HalberdAndShield)
        {
            return "Halberd and Shield";
        }
        else if (parameter.intValue == GreatBlade)
        {
            return "Greatblade";
        }
        else if (parameter.intValue == PairedHandWeapons)
        {
            return "Paired Hand Weapons";
        }
    }
    return SlavesToDarknessBase::ValueToString(parameter);
}

int ChaosWarriors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Hand Weapon and Shield")
    {
        return HandWeaponAndShield;
    }
    else if (enumString == "Halberd and Shield")
    {
        return HalberdAndShield;
    }
    else if (enumString == "Greatblade")
    {
        return GreatBlade;
    }
    else if (enumString == "Paired Hand Weapons")
    {
        return PairedHandWeapons;
    }
    return SlavesToDarknessBase::EnumStringToInt(enumString);
}

void ChaosWarriors::onWounded()
{
    Unit::onWounded();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->slain() && (ip->getName() == "Hornblower"))
        {
            m_hornblower = false;
        }
        if (ip->slain() && (ip->getName() == "Standard Bearer"))
        {
            m_standardBearer = false;
        }
    }
}

Wounds ChaosWarriors::applyWoundSave(const Wounds &wounds)
{
    if (m_hasShields)
    {
        Dice dice;

        // Chaos Runeshield
        Dice::RollResult mortalSaves;
        dice.rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }
    return Unit::applyWoundSave(wounds);
}

int ChaosWarriors::runModifier() const
{
    auto modifier = Unit::runModifier();
    if (m_hornblower) modifier += 1;
    return modifier;
}

int ChaosWarriors::chargeModifier() const
{
    auto modifier = Unit::chargeModifier();
    if (m_hornblower) modifier += 1;
    return modifier;
}

int ChaosWarriors::braveryModifier() const
{
    auto modifier = Unit::braveryModifier();
    if (m_standardBearer) modifier += 1;
    return modifier;
}

Rerolls ChaosWarriors::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Pair of Chaos Hand Weapons
    if (m_pairedWeapons)
        return RerollOnes;
    return Unit::toHitRerolls(weapon, target);
}

Rerolls ChaosWarriors::toSaveRerolls(const Weapon *weapon) const
{
    // Legions of Chaos
    if (remainingModels() >= 10)
        return RerollFailed;
    return Unit::toSaveRerolls(weapon);
}

void ChaosWarriors::onRestore()
{
    Unit::onRestore();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->getName() == "Hornblower")
        {
            m_hornblower = true;
        }
        if (ip->getName() == "Standard Bearer")
        {
            m_standardBearer = true;
        }
    }
}

int ChaosWarriors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //SlavesToDarkness