/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/GraveGuard.h>
#include <UnitFactory.h>

namespace Death
{
static const int BASESIZE = 25;
static const int WOUNDS = 1;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 30;
static const int POINTS_PER_BLOCK = 140;
static const int POINTS_MAX_UNIT_SIZE = 360;

bool GraveGuard::s_registered = false;

GraveGuard::GraveGuard() :
    LegionOfNagashBase("Grave Guard", 4, WOUNDS, 10, 5, false),
    m_wightBlade(Weapon::Type::Melee, "Wight Blade", 1, 2, 3, 4, -1, 1),
    m_wightBladeSeneschal(Weapon::Type::Melee, "Wight Blade", 1, 3, 3, 4, -1, 1),
    m_greatWightBlade(Weapon::Type::Melee, "Great Wight Blade", 1, 2, 3, 3, -1, 1),
    m_greatWightBladeSeneschal(Weapon::Type::Melee, "Great Wight Blade", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, GRAVE_GUARD};
    m_weapons = {&m_wightBlade, &m_wightBladeSeneschal, &m_greatWightBlade, &m_greatWightBladeSeneschal};
}

bool GraveGuard::configure(int numModels, GraveGuard::WeaponOptions weapons, bool standardBearers, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearers = standardBearers;
    // TODO: units with hornblowers min charge distance is 6".
    m_hornblowers = hornblowers;
    m_weaponOption = weapons;

    auto seneschal = new Model(BASESIZE, wounds());
    if (weapons == WightBlade)
    {
        seneschal->addMeleeWeapon(&m_wightBladeSeneschal);
    }
    else if (weapons == GreatWightBlade)
    {
        seneschal->addMeleeWeapon(&m_greatWightBladeSeneschal);
    }
    addModel(seneschal);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        if (weapons == WightBlade)
        {
            model->addMeleeWeapon(&m_wightBlade);
        }
        else if (weapons == GreatWightBlade)
        {
            model->addMeleeWeapon(&m_greatWightBlade);
        }
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *GraveGuard::Create(const ParameterList &parameters)
{
    auto unit = new GraveGuard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, WightBlade);
    bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

    bool ok = unit->configure(numModels, weapons, standardBearers, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GraveGuard::Init()
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
                {ParamType::Enum, "Weapons", WightBlade, WightBlade, GreatWightBlade, 1},
                {ParamType::Boolean, "Standard Bearers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                {ParamType::Boolean, "Hornblowers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
            },
            DEATH,
            { DEATHRATTLE }
        };
        s_registered = UnitFactory::Register("Grave Guard", factoryMethod);
    }
}

std::string GraveGuard::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == WightBlade)
        { return "Wight Blade"; }
        else if (parameter.intValue == GreatWightBlade)
        { return "Great Wight Blade"; }
    }
    return ParameterValueToString(parameter);
}

int GraveGuard::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Wight Blade") { return WightBlade; }
    else if (enumString == "Great Wight Blade") { return GreatWightBlade; }
    return 0;
}

int GraveGuard::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Crypt Shields
    if (weapon->rend() == 0)
    {
        modifier += 1;
    }

    return modifier;
}

Wounds GraveGuard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    if (m_charged && (woundRoll >= 6))
    {
        // Cursed Weapons 2x damage on 6+
        return {weapon->damage() + weapon->damage(), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int GraveGuard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Death
