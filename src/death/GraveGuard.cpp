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
static FactoryMethod factoryMethod = {
    GraveGuard::Create,
    GraveGuard::ValueToString,
    GraveGuard::EnumStringToInt,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = GraveGuard::MIN_UNIT_SIZE}, GraveGuard::MIN_UNIT_SIZE, GraveGuard::MAX_UNIT_SIZE,
            GraveGuard::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "standardBearers", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = false}, false, false, false},
    },
    DEATH,
    DEATHRATTLE
};

bool GraveGuard::s_registered = false;

GraveGuard::GraveGuard() :
    Unit("Grave Guard", 4, WOUNDS, 10, 5, false),
    m_wightBlade(Weapon::Type::Melee, "Wight Blade", 1, 2, 3, 4, -1, 1),
    m_wightBladeSeneschal(Weapon::Type::Melee, "Wight Blade (Seneschal)", 1, 3, 3, 4, -1, 1),
    m_greatWightBlade(Weapon::Type::Melee, "Great Wight Blade", 1, 2, 3, 3, -1, 1),
    m_greatWightBladeSeneschal(Weapon::Type::Melee, "Great Wight Blade (Seneschal)", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, GRAVE_GUARD};
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
    m_weapons = weapons;

    Model seneschal(BASESIZE, WOUNDS);
    if (weapons == WightBlade)
    {
        seneschal.addMeleeWeapon(&m_wightBladeSeneschal);
    }
    else if (weapons == GreatWightBlade)
    {
        seneschal.addMeleeWeapon(&m_greatWightBladeSeneschal);
    }
    addModel(seneschal);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == WightBlade)
        {
            model.addMeleeWeapon(&m_wightBlade);
        }
        else if (weapons == GreatWightBlade)
        {
            model.addMeleeWeapon(&m_greatWightBlade);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void GraveGuard::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_wightBlade);
    visitor(&m_wightBladeSeneschal);
    visitor(&m_greatWightBlade);
    visitor(&m_greatWightBladeSeneschal);
}

Unit *GraveGuard::Create(const ParameterList &parameters)
{
    auto unit = new GraveGuard();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapons = (WeaponOptions) GetIntParam("weapons", parameters, WightBlade);
    bool standardBearers = GetBoolParam("standardBearers", parameters, false);
    bool hornblowers = GetBoolParam("hornblowers", parameters, false);

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
        s_registered = UnitFactory::Register("Grave Guard", factoryMethod);
    }
}

std::string GraveGuard::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == WightBlade)
        { return "WightBlade"; }
        else if (parameter.m_intValue == GreatWightBlade)
        { return "GreatWightBlade"; }
    }
    return ParameterValueToString(parameter);
}

int GraveGuard::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "WightBlade")
    { return WightBlade; }
    else if (enumString == "GreatWightBlade")
    { return GreatWightBlade; }
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

} //namespace Death
