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
            ParamType::Enum, "Weapons", ChaosMarauders::Axe, ChaosMarauders::Flail,
            ChaosMarauders::Axe, 1
        },
        {ParamType::Boolean, "Shields", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Damned Icon", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Tribal Banner", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
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
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, Axe);
    bool shields = GetBoolParam("Shields", parameters, true);
    bool damnedIcon = GetBoolParam("Damned Icon", parameters, false);
    bool tribalBanner = GetBoolParam("Tribal Banner", parameters, false);
    bool drummer = GetBoolParam("Drummer", parameters, false);
    bool ok = unit->configure(numModels, weapons, shields, damnedIcon, tribalBanner, drummer);
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
    m_axe(Weapon::Type::Melee, "Barbarian Axe", 1, 1, 4, 4, 0, 1),
    m_flail(Weapon::Type::Melee, "Barbarian Flail", 1, 1, 5, 3, 0, 1),
    m_axeChieftain(Weapon::Type::Melee, "Barbarian Axe", 1, 2, 4, 4, 0, 1),
    m_flailChieftain(Weapon::Type::Melee, "Barbarian Flail", 1, 2, 5, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CHAOS_MARAUDERS};
}

bool ChaosMarauders::configure(int numModels, WeaponOption weapons, bool shields, bool damnedIcon, bool tribalBanner, bool drummer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_hasShields = shields;
    m_damnedIcon = damnedIcon;
    m_tribalBanner = tribalBanner;
    m_drummer = drummer;

    Model leader(BASESIZE, WOUNDS);
    if (weapons == Axe)
    {
        leader.addMeleeWeapon(&m_axeChieftain);
    }
    else if (weapons == Flail)
    {
        leader.addMeleeWeapon(&m_flailChieftain);
    }
    leader.setName("Marauder Chieftain");
    addModel(leader);

    if (m_damnedIcon)
    {
        Model model(BASESIZE, WOUNDS);
        model.setName("Damned Icon Bearer");
        if (weapons == Axe)
            model.addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model.addMeleeWeapon(&m_flail);
        addModel(model);
    }

    if (m_tribalBanner)
    {
        Model model(BASESIZE, WOUNDS);
        model.setName("Tribal Banner Bearer");
        if (weapons == Axe)
            model.addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model.addMeleeWeapon(&m_flail);
        addModel(model);
    }

    if (m_drummer)
    {
        Model model(BASESIZE, WOUNDS);
        model.setName("Drummer");
        if (weapons == Axe)
            model.addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model.addMeleeWeapon(&m_flail);
        addModel(model);
    }

    for (auto i = (int)m_models.size(); i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == Axe)
            model.addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model.addMeleeWeapon(&m_flail);
        addModel(model);
    }

    if (m_hasShields)
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

void ChaosMarauders::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_axe);
    visitor(&m_flail);
    visitor(&m_axeChieftain);
    visitor(&m_flailChieftain);
}

std::string ChaosMarauders::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == Axe)
        {
            return "Barbarian Axe";
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
    if (enumString == "Barbarian Axe")
    {
        return Axe;
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
        if (ip.slain() && (ip.getName() == "Drummer"))
        {
            m_drummer = false;
        }
        if (ip.slain() && (ip.getName() == "Damned Icon Bearer"))
        {
            m_damnedIcon = false;
        }
        if (ip.slain() && (ip.getName() == "Tribal Banner Bearer"))
        {
            m_tribalBanner = false;
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

int ChaosMarauders::braveryModifier() const
{
    auto modifier = Unit::braveryModifier();
    if (m_tribalBanner) modifier += 1;
    return modifier;
}

void ChaosMarauders::onRestore()
{
    Unit::onRestore();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip.getName() == "Drummer")
        {
            m_drummer = true;
        }
        if (ip.getName() == "Damned Icon Bearer")
        {
            m_damnedIcon = true;
        }
        if (ip.getName() == "Tribal Banner Bearer")
        {
            m_tribalBanner = true;
        }
    }
}

Rerolls ChaosMarauders::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if (m_damnedIcon)
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, target);
}

void ChaosMarauders::onStartCombat(PlayerId player)
{
    Dice dice;

    m_hordeWoundModifier = 0;
    m_hordeHitModifier = 0;

    // Barbarian Horde
    int roll = dice.rollD6();
    if (remainingModels() >= 20)
        roll++;

    if (roll >= 4)
        m_hordeHitModifier = 1;

    if (roll >= 6)
        m_hordeWoundModifier = 1;

    Unit::onStartCombat(player);
}

int ChaosMarauders::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto modifier = Unit::toHitModifier(weapon, target);
    return m_hordeHitModifier + modifier;
}

int ChaosMarauders::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    auto modifier = Unit::toWoundModifier(weapon, target);
    return m_hordeWoundModifier + modifier;
}

} //SlavesToDarkness