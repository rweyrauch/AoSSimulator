/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosKnights.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosKnights::Create,
    ChaosKnights::ValueToString,
    ChaosKnights::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", ChaosKnights::MIN_UNIT_SIZE, ChaosKnights::MIN_UNIT_SIZE,
            ChaosKnights::MAX_UNIT_SIZE, ChaosKnights::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", ChaosKnights::EnsorcelledWeapon, ChaosKnights::EnsorcelledWeapon,
            ChaosKnights::ChaosGlaive, 1
        },
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool ChaosKnights::s_registered = false;

Unit *ChaosKnights::Create(const ParameterList &parameters)
{
    auto unit = new ChaosKnights();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, EnsorcelledWeapon);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    bool ok = unit->configure(numModels, weapons, standardBearer, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosKnights::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Knights", factoryMethod);
    }
}

ChaosKnights::ChaosKnights() :
    Unit("Chaos Knights", 10, WOUNDS, 7, 4, false),
    m_ensorcelledWeapon(Weapon::Type::Melee, "Ensorcelled Weapon", 1, 3, 3, 4, 0, 1),
    m_glaive(Weapon::Type::Melee, "Chaos Glaive", 1, 2, 4, 3, 0, 1),
    m_ensorcelledWeaponLeader(Weapon::Type::Melee, "Ensorcelled Weapon (Doom Knight)", 1, 3, 2, 4, 0, 1),
    m_glaiveLeader(Weapon::Type::Melee, "Chaos Glaive (Doom Knight)", 1, 2, 3, 3, 0, 1),
    m_hooves(Weapon::Type::Melee, "War Steed's Roughshod Hooves", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CHAOS_KNIGHTS};
}

bool ChaosKnights::configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    Model leader(BASESIZE, WOUNDS);
    if (weapons == EnsorcelledWeapon)
    {
        leader.addMeleeWeapon(&m_ensorcelledWeaponLeader);
    }
    else if (weapons == ChaosGlaive)
    {
        leader.addMeleeWeapon(&m_glaiveLeader);
    }
    leader.addMeleeWeapon(&m_hooves);
    leader.setName("Doom Knight");
    addModel(leader);

    if (m_standardBearer)
    {
        Model model(BASESIZE, WOUNDS);
        model.setName("Standard Bearer");
        if (weapons == EnsorcelledWeapon)
            model.addMeleeWeapon(&m_ensorcelledWeapon);
        else if (weapons == ChaosGlaive)
            model.addMeleeWeapon(&m_glaive);
        model.addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    if (m_hornblower)
    {
        Model model(BASESIZE, WOUNDS);
        model.setName("Hornblower");
        if (weapons == EnsorcelledWeapon)
            model.addMeleeWeapon(&m_ensorcelledWeapon);
        else if (weapons == ChaosGlaive)
            model.addMeleeWeapon(&m_glaive);
        model.addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    for (auto i = (int)m_models.size(); i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == EnsorcelledWeapon)
            model.addMeleeWeapon(&m_ensorcelledWeapon);
        else if (weapons == ChaosGlaive)
            model.addMeleeWeapon(&m_glaive);
        model.addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void ChaosKnights::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_ensorcelledWeapon);
    visitor(&m_glaive);
    visitor(&m_ensorcelledWeaponLeader);
    visitor(&m_glaiveLeader);
    visitor(&m_hooves);
}

std::string ChaosKnights::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == EnsorcelledWeapon)
        {
            return "Ensorcelled Weapon";
        }
        else if (parameter.m_intValue == ChaosGlaive)
        {
            return "Chaos Glaive";
        }
    }
    return ParameterValueToString(parameter);
}

int ChaosKnights::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Chaos Glaive")
    {
        return ChaosGlaive;
    }
    else if (enumString == "Chaos Glaive")
    {
        return ChaosGlaive;
    }
    return 0;
}

void ChaosKnights::onWounded()
{
    Unit::onWounded();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip.slain() && (ip.getName() == "Hornblower"))
        {
            m_hornblower = false;
        }
        if (ip.slain() && (ip.getName() == "Standard Bearer"))
        {
            m_standardBearer = false;
        }
    }
}

Wounds ChaosKnights::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    // Chaos Runeshield
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

int ChaosKnights::runModifier() const
{
    auto modifier = Unit::runModifier();
    if (m_hornblower) modifier += 1;
    return modifier;
}

int ChaosKnights::chargeModifier() const
{
    auto modifier = Unit::chargeModifier();
    if (m_hornblower) modifier += 1;
    return modifier;
}

int ChaosKnights::braveryModifier() const
{
    auto modifier = Unit::braveryModifier();
    if (m_standardBearer) modifier += 1;
    return modifier;
}

void ChaosKnights::onRestore()
{
    Unit::onRestore();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip.getName() == "Hornblower")
        {
            m_hornblower = true;
        }
        if (ip.getName() == "Standard Bearer")
        {
            m_standardBearer = true;
        }
    }
}

Wounds ChaosKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Impaling Charge
    if (m_charged && (weapon->name() == m_glaive.name()))
    {
        return { 2, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int ChaosKnights::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Impaling Charge
    if (m_charged && (weapon->name() == m_glaive.name()))
    {
        return -1;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

} //SlavesToDarkness