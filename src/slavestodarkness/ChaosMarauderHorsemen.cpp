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
static const int BASESIZE = 65;
static const int WOUNDS = 2;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 30;
static const int POINTS_PER_BLOCK = 90;
static const int POINTS_MAX_UNIT_SIZE = 270;

bool ChaosMarauderHorsemen::s_registered = false;

Unit *ChaosMarauderHorsemen::Create(const ParameterList &parameters)
{
    auto unit = new ChaosMarauderHorsemen();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, AxeAndShield);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    auto mark = (MarkOfChaos)GetEnumParam("Mark of Chaos", parameters, Undivided);
    unit->setMarkOfChaos(mark);

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
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Weapons", AxeAndShield, AxeAndShield, Flail, 1},
                {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
            },
            CHAOS,
            { SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE }
        };

        s_registered = UnitFactory::Register("Chaos Marauders", factoryMethod);
    }
}

ChaosMarauderHorsemen::ChaosMarauderHorsemen() :
    SlavesToDarknessBase("Chaos Marauder Horsemen", 12, WOUNDS, 5, 6, false),
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
    m_weapons = {&m_javelinMissile, &m_axe, &m_flail, &m_javelin, &m_axeMaster, &m_flailMaster, &m_javelinMaster, &m_hooves};

    s_globalBraveryMod.connect(this, &ChaosMarauderHorsemen::iconBearer, &m_connection);

    // Feigned Flight
    m_retreatAndCharge = true;
    m_retreatAndShoot = true;
}

ChaosMarauderHorsemen::~ChaosMarauderHorsemen()
{
    m_connection.disconnect();
}

bool ChaosMarauderHorsemen::configure(int numModels, WeaponOption weapons, bool iconBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weaponOption = weapons;
    m_iconBearer = iconBearer;
    m_hornblower = hornblower;

    auto leader = new Model(BASESIZE, wounds());
    if (weapons == AxeAndShield)
    {
        leader->addMeleeWeapon(&m_axeMaster);
    }
    else if (weapons == Flail)
    {
        leader->addMeleeWeapon(&m_flailMaster);
    }
    else if (weapons == JavelinAndShield)
    {
        leader->addMeleeWeapon(&m_javelinMaster);
        leader->addMissileWeapon(&m_javelinMissile);
    }
    leader->addMeleeWeapon(&m_hooves);
    leader->setName("Marauder Chieftain");
    addModel(leader);

    if (m_iconBearer)
    {
        auto model = new Model(BASESIZE, wounds());
        model->setName("Icon Bearer");
        if (weapons == AxeAndShield)
            model->addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model->addMeleeWeapon(&m_flail);
        else if (weapons == JavelinAndShield)
        {
            model->addMissileWeapon(&m_javelinMissile);
            model->addMeleeWeapon(&m_javelin);
        }
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    if (m_hornblower)
    {
        auto model = new Model(BASESIZE, wounds());
        model->setName("Hornblower");
        if (weapons == AxeAndShield)
            model->addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model->addMeleeWeapon(&m_flail);
        else if (weapons == JavelinAndShield)
        {
            model->addMissileWeapon(&m_javelinMissile);
            model->addMeleeWeapon(&m_javelin);
        }
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    for (auto i = (int)m_models.size(); i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        if (weapons == AxeAndShield)
            model->addMeleeWeapon(&m_axe);
        else if (weapons == Flail)
            model->addMeleeWeapon(&m_flail);
        else if (weapons == JavelinAndShield)
        {
            model->addMissileWeapon(&m_javelinMissile);
            model->addMeleeWeapon(&m_javelin);
        }
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    // Darkwood Shields
    if (m_weaponOption == AxeAndShield || m_weaponOption == JavelinAndShield)
    {
        m_save = 5;
    }

    m_points = ComputePoints(numModels);

    return true;
}

std::string ChaosMarauderHorsemen::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == AxeAndShield)
        {
            return "Barbarian Axe and Darkwood Shield";
        }
        else if (parameter.intValue == Flail)
        {
            return "Barbarian Flail";
        }
        else if (parameter.intValue == JavelinAndShield)
        {
            return "Marauder Javelin and Darkwood Shield";
        }
    }
    return SlavesToDarknessBase::ValueToString(parameter);
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
    return SlavesToDarknessBase::EnumStringToInt(enumString);
}

void ChaosMarauderHorsemen::onWounded()
{
    Unit::onWounded();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->slain() && (ip->getName() == "Hornblower"))
        {
            m_hornblower = false;
        }
        if (ip->slain() && (ip->getName() == "Icon Bearer"))
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
        if (ip->getName() == "Hornblower")
        {
            m_hornblower = true;
        }
        if (ip->getName() == "Icon Bearer")
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

int ChaosMarauderHorsemen::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int ChaosMarauderHorsemen::iconBearer(const Unit *unit)
{
    // Icon Bearer
    if (m_iconBearer && (unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 6.0f))
    {
        return -1;
    }
    return 0;
}

} //SlavesToDarkness