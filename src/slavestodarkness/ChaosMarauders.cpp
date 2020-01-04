/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosMarauders.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosMarauders::Create,
    ChaosMarauders::ValueToString,
    ChaosMarauders::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", ChaosMarauders::MIN_UNIT_SIZE, ChaosMarauders::MIN_UNIT_SIZE,
            ChaosMarauders::MAX_UNIT_SIZE, ChaosMarauders::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", ChaosMarauders::AxeAndShield, ChaosMarauders::Flail,
            ChaosMarauders::AxeAndShield, 1
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Drummer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool ChaosMarauders::s_registered = false;

Unit *ChaosMarauders::Create(const ParameterList &parameters)
{
    auto unit = new ChaosMarauders();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, AxeAndShield);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool drummer = GetBoolParam("Drummer", parameters, false);
    bool ok = unit->configure(numModels, weapons, iconBearer, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosMarauders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Marauders", factoryMethod);
    }
}

ChaosMarauders::ChaosMarauders() :
    Unit("Chaos Marauders", 6, WOUNDS, 5, 6, false),
    m_axe(Weapon::Type::Melee, "Barbarian Axe", 1, 2, 4, 4, 0, 1),
    m_flail(Weapon::Type::Melee, "Barbarian Flail", 2, 1, 4, 3, 0, 1),
    m_axeChieftain(Weapon::Type::Melee, "Barbarian Axe", 1, 3, 4, 4, 0, 1),
    m_flailChieftain(Weapon::Type::Melee, "Barbarian Flail", 2, 2, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_MARAUDERS};
}

bool ChaosMarauders::configure(int numModels, WeaponOption weapons, bool iconBearer, bool drummer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weapons = weapons;
    m_iconBearer = iconBearer;
    m_drummer = drummer;

    auto leader = new Model(BASESIZE, WOUNDS);
    if (weapons == AxeAndShield)
    {
        leader->addMeleeWeapon(&m_axeChieftain);
    }
    else if (weapons == Flail)
    {
        leader->addMeleeWeapon(&m_flailChieftain);
    }
    leader->setName("Marauder Chieftain");
    addModel(leader);

    if (m_iconBearer)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Icon Bearer");
        if (weapons == AxeAndShield)
            model->addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model->addMeleeWeapon(&m_flail);
        addModel(model);
    }

    if (m_drummer)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Drummer");
        if (weapons == AxeAndShield)
            model->addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model->addMeleeWeapon(&m_flail);
        addModel(model);
    }

    for (auto i = (int)m_models.size(); i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == AxeAndShield)
            model->addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model->addMeleeWeapon(&m_flail);
        addModel(model);
    }

    // Darkwood Shields
    if (m_weapons == WeaponOption::AxeAndShield)
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

void ChaosMarauders::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_axe);
    visitor(m_flail);
    visitor(m_axeChieftain);
    visitor(m_flailChieftain);
}

std::string ChaosMarauders::ValueToString(const Parameter &parameter)
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
    }
    return ParameterValueToString(parameter);
}

int ChaosMarauders::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Barbarian Axe and Darkwood Shield")
    {
        return AxeAndShield;
    }
    else if (enumString == "Barbarian Flail")
    {
        return Flail;
    }
    return 0;
}

void ChaosMarauders::onWounded()
{
    Unit::onWounded();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->slain() && (ip->getName() == "Drummer"))
        {
            m_drummer = false;
        }
        if (ip->slain() && (ip->getName() == "Icon Bearer"))
        {
            m_iconBearer = false;
        }
    }
}

int ChaosMarauders::runModifier() const
{
    auto modifier = Unit::runModifier();
    if (m_drummer) modifier += 1;
    return modifier;
}

int ChaosMarauders::chargeModifier() const
{
    auto modifier = Unit::chargeModifier();
    if (m_drummer) modifier += 1;
    return modifier;
}

void ChaosMarauders::onRestore()
{
    Unit::onRestore();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->getName() == "Drummer")
        {
            m_drummer = true;
        }
        if (ip->getName() == "Icon Bearer")
        {
            m_iconBearer = true;
        }
    }
}

int ChaosMarauders::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    // Barbarian Hordes
    auto modifier = Unit::toHitModifier(weapon, target);
    if (remainingModels() >= 10) modifier++;
    return modifier;
}

int ChaosMarauders::weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const
{
    // Barbarian Hordes
    auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    if (remainingModels() >= 20) rend--;
    return rend;
}

} //SlavesToDarkness