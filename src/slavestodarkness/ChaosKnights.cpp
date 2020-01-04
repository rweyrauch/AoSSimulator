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
            ChaosKnights::CursedLance, 1
        },
        {
            ParamType::Enum, "Doom Knight Weapon", ChaosKnights::EnsorcelledWeapon, ChaosKnights::EnsorcelledWeapon,
            ChaosKnights::CursedFlail, 1
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
    auto doomWeapon = (WeaponOption)GetEnumParam("Doom Knight Weapon", parameters, EnsorcelledWeapon);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    bool ok = unit->configure(numModels, weapons, doomWeapon, standardBearer, hornblower);
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
    m_ensorcelledWeapon(Weapon::Type::Melee, "Ensorcelled Weapon", 1, 3, 3, 3, -1, 1),
    m_lance(Weapon::Type::Melee, "Cursed Lance", 2, 2, 4, 3, 0, 1),
    m_ensorcelledWeaponLeader(Weapon::Type::Melee, "Ensorcelled Weapon", 1, 4, 3, 3, -1, 1),
    m_lanceLeader(Weapon::Type::Melee, "Cursed Lance", 2, 3, 4, 3, 0, 1),
    m_flailLeader(Weapon::Type::Melee, "Cursed Flail", 2, RAND_D6, 4, 3, 0, 1),
    m_hooves(Weapon::Type::Melee, "Trampling Hooves", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_KNIGHTS};
}

bool ChaosKnights::configure(int numModels, WeaponOption weapons, WeaponOption doomKnightWeapon, bool standardBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    if (weapons == CursedFlail)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    auto leader = new Model(BASESIZE, WOUNDS);
    if (doomKnightWeapon == EnsorcelledWeapon)
    {
        leader->addMeleeWeapon(&m_ensorcelledWeaponLeader);
    }
    else if (doomKnightWeapon == CursedLance)
    {
        leader->addMeleeWeapon(&m_lanceLeader);
    }
    else if (doomKnightWeapon == CursedFlail)
    {
        leader->addMeleeWeapon(&m_flailLeader);
    }
    leader->addMeleeWeapon(&m_hooves);
    leader->setName("Doom Knight");
    addModel(leader);

    if (m_standardBearer)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Standard Bearer");
        if (weapons == EnsorcelledWeapon)
            model->addMeleeWeapon(&m_ensorcelledWeapon);
        else if (weapons == CursedLance)
            model->addMeleeWeapon(&m_lance);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    if (m_hornblower)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Hornblower");
        if (weapons == EnsorcelledWeapon)
            model->addMeleeWeapon(&m_ensorcelledWeapon);
        else if (weapons == CursedLance)
            model->addMeleeWeapon(&m_lance);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    for (auto i = (int)m_models.size(); i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == EnsorcelledWeapon)
            model->addMeleeWeapon(&m_ensorcelledWeapon);
        else if (weapons == CursedLance)
            model->addMeleeWeapon(&m_lance);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void ChaosKnights::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_ensorcelledWeapon);
    visitor(m_lance);
    visitor(m_ensorcelledWeaponLeader);
    visitor(m_lanceLeader);
    visitor(m_flailLeader);
    visitor(m_hooves);
}

std::string ChaosKnights::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons" || parameter.m_name == "Doom Knight Weapon")
    {
        if (parameter.m_intValue == EnsorcelledWeapon)
        {
            return "Ensorcelled Weapon";
        }
        else if (parameter.m_intValue == CursedLance)
        {
            return "Cursed Lance";
        }
        else if (parameter.m_intValue == CursedFlail)
        {
            return "Cursed Flail";
        }
    }
    return ParameterValueToString(parameter);
}

int ChaosKnights::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Ensorcelled Weapon")
    {
        return EnsorcelledWeapon;
    }
    else if (enumString == "Cursed Lance")
    {
        return CursedLance;
    }
    else if (enumString == "Cursed Flail")
    {
        return CursedFlail;
    }
    return 0;
}

void ChaosKnights::onWounded()
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

Wounds ChaosKnights::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    // Chaos Runeshield
    Dice::RollResult woundSaves, mortalSaves;
    dice.rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
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

Wounds ChaosKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Impaling Charge
    if (m_charged && (weapon->name() == m_lance.name()))
    {
        return { weapon->damage()+1, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int ChaosKnights::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Impaling Charge
    if (m_charged && (weapon->name() == m_lance.name()))
    {
        return weapon->rend()-2;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

} //SlavesToDarkness