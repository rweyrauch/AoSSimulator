/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosMarauderHorsemen.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosMarauderHorsemen::Create,
    ChaosMarauderHorsemen::ValueToString,
    ChaosMarauderHorsemen::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", ChaosMarauderHorsemen::MIN_UNIT_SIZE, ChaosMarauderHorsemen::MIN_UNIT_SIZE,
            ChaosMarauderHorsemen::MAX_UNIT_SIZE, ChaosMarauderHorsemen::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", ChaosMarauderHorsemen::AxeAndShield, ChaosMarauderHorsemen::AxeAndShield,
            ChaosMarauderHorsemen::Flail, 1
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool ChaosMarauderHorsemen::s_registered = false;

Unit *ChaosMarauderHorsemen::Create(const ParameterList &parameters)
{
    auto unit = new ChaosMarauderHorsemen();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, AxeAndShield);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    bool ok = unit->configure(numModels, weapons, iconBearer, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosMarauderHorsemen::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Marauders", factoryMethod);
    }
}

ChaosMarauderHorsemen::ChaosMarauderHorsemen() :
    Unit("Chaos Marauder Horsemen", 12, WOUNDS, 5, 6, false),
    m_javelinMissile(Weapon::Type::Missile, "Marauder Javelin", 12, 1, 4, 3, -1, 1),
    m_axe(Weapon::Type::Melee, "Barbarian Axe", 1, 2, 4, 4, 0, 1),
    m_flail(Weapon::Type::Melee, "Barbarian Flail", 2, 1, 4, 3, -1, 1),
    m_javelin(Weapon::Type::Melee, "Marauder Javelin", 2, 1, 4, 3, 0, 1),
    m_axeMaster(Weapon::Type::Melee, "Barbarian Axe", 1, 3, 4, 4, 0, 1),
    m_flailMaster(Weapon::Type::Melee, "Barbarian Flail", 2, 2, 4, 3, -1, 1),
    m_javelinMaster(Weapon::Type::Melee, "Marauder Javelin", 2, 2, 4, 3, 0, 1),
    m_hooves(Weapon::Type::Melee, "Trampling Hooves", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_MARAUDER_HORSEMEN};

    // Feigned Flight
    m_retreatAndCharge = true;
    m_retreatAndShoot = true;
}

bool ChaosMarauderHorsemen::configure(int numModels, WeaponOption weapons, bool iconBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weapons = weapons;
    m_iconBearer = iconBearer;
    m_hornblower = hornblower;

    Model leader(BASESIZE, WOUNDS);
    if (weapons == AxeAndShield)
    {
        leader.addMeleeWeapon(&m_axeMaster);
    }
    else if (weapons == Flail)
    {
        leader.addMeleeWeapon(&m_flailMaster);
    }
    else if (weapons == JavelinAndShield)
    {
        leader.addMeleeWeapon(&m_javelinMaster);
        leader.addMissileWeapon(&m_javelinMissile);
    }
    leader.addMeleeWeapon(&m_hooves);
    leader.setName("Marauder Chieftain");
    addModel(leader);

    if (m_iconBearer)
    {
        Model model(BASESIZE, WOUNDS);
        model.setName("Icon Bearer");
        if (weapons == AxeAndShield)
            model.addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model.addMeleeWeapon(&m_flail);
        else if (weapons == JavelinAndShield)
        {
            model.addMissileWeapon(&m_javelinMissile);
            model.addMeleeWeapon(&m_javelin);
        }
        model.addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    if (m_hornblower)
    {
        Model model(BASESIZE, WOUNDS);
        model.setName("Hornblower");
        if (weapons == AxeAndShield)
            model.addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model.addMeleeWeapon(&m_flail);
        else if (weapons == JavelinAndShield)
        {
            model.addMissileWeapon(&m_javelinMissile);
            model.addMeleeWeapon(&m_javelin);
        }
        model.addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    for (auto i = (int)m_models.size(); i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == AxeAndShield)
            model.addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model.addMeleeWeapon(&m_flail);
        else if (weapons == JavelinAndShield)
        {
            model.addMissileWeapon(&m_javelinMissile);
            model.addMeleeWeapon(&m_javelin);
        }
        model.addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    // Darkwood Shields
    if (m_weapons == AxeAndShield || m_weapons == JavelinAndShield)
    {
        m_save = 5;
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void ChaosMarauderHorsemen::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_javelinMissile);
    visitor(m_axe);
    visitor(m_flail);
    visitor(m_javelin);
    visitor(m_axeMaster);
    visitor(m_flailMaster);
    visitor(m_javelinMaster);
}

std::string ChaosMarauderHorsemen::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == AxeAndShield)
        {
            return "Barbarian Axe and Darkwood Shield";
        }
        else if (parameter.m_intValue == Flail)
        {
            return "Barbarian Flail";
        }
        else if (parameter.m_intValue == JavelinAndShield)
        {
            return "Marauder Javelin and Darkwood Shield";
        }
    }
    return ParameterValueToString(parameter);
}

int ChaosMarauderHorsemen::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Barbarian Axe and Darkwood Shield")
    {
        return AxeAndShield;
    }
    else if (enumString == "Barbarian Flail")
    {
        return Flail;
    }
    else if (enumString == "Marauder Javelin and Darkwood Shield")
    {
        return JavelinAndShield;
    }
    return 0;
}

void ChaosMarauderHorsemen::onWounded()
{
    Unit::onWounded();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip.slain() && (ip.getName() == "Hornblower"))
        {
            m_hornblower = false;
        }
        if (ip.slain() && (ip.getName() == "Icon Bearer"))
        {
            m_iconBearer = false;
        }
    }
}

int ChaosMarauderHorsemen::runModifier() const
{
    auto modifier = Unit::runModifier();
    if (m_hornblower) modifier += 1;
    return modifier;
}

int ChaosMarauderHorsemen::chargeModifier() const
{
    auto modifier = Unit::chargeModifier();
    if (m_hornblower) modifier += 1;
    return modifier;
}

void ChaosMarauderHorsemen::onRestore()
{
    Unit::onRestore();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip.getName() == "Hornblower")
        {
            m_hornblower = true;
        }
        if (ip.getName() == "Icon Bearer")
        {
            m_iconBearer = true;
        }
    }
}

int ChaosMarauderHorsemen::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    // Barbarian Hordes
    auto modifier = Unit::toHitModifier(weapon, target);
    if (remainingModels() >= 10) modifier++;
    return modifier;
}

int ChaosMarauderHorsemen::weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const
{
    // Barbarian Hordes
    auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    if (remainingModels() >= 10) rend--;
    return rend;
}

} //SlavesToDarkness