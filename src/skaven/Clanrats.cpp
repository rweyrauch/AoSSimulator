/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Clanrats.h>
#include <UnitFactory.h>

namespace Skaven
{
static FactoryMethod factoryMethod = {
    Clanrats::Create,
    Clanrats::ValueToString,
    Clanrats::EnumStringToInt,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Clanrats::MIN_UNIT_SIZE}, Clanrats::MIN_UNIT_SIZE,
            Clanrats::MAX_UNIT_SIZE, Clanrats::MIN_UNIT_SIZE
        },
        {
            ParamType::Integer, "weapons", {.m_intValue = Clanrats::RustySpear}, Clanrats::RustySpear,
            Clanrats::RustyBlade, 1
        },
        {ParamType::Boolean, "clanshields", {.m_boolValue = false}, false, false, false},
        {ParamType::Integer, "standardBearers", {.m_intValue = 0}, 0, Clanrats::MAX_UNIT_SIZE/Clanrats::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "bellRinders", {.m_intValue = 0}, 0, Clanrats::MAX_UNIT_SIZE/Clanrats::MIN_UNIT_SIZE, 1}
    },
    CHAOS,
    SKAVEN
};

bool Clanrats::s_registered = false;

Clanrats::Clanrats() :
    Unit("Clanrats", 6, WOUNDS, 4, 6, false),
    m_rustySpear(Weapon::Type::Melee, "Rusty Spear", 2, 1, 5, 4, 0, 1),
    m_rustySpearLeader(Weapon::Type::Melee, "Rusty Spear (Leader)", 2, 2, 5, 4, 0, 1),
    m_rustyBlade(Weapon::Type::Melee, "Rusty Blade", 1, 1, 4, 4, 0, 1),
    m_rustyBladeLeader(Weapon::Type::Melee, "Rusty Blade (Leader)", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, CLANRATS};
}

bool Clanrats::configure(int numModels, Clanrats::WeaponOptions weapons, bool clanshields, int standardBearers, int bellRingers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    int maxStandardBearers = numModels / MIN_UNIT_SIZE;
    int maxBellRingers = numModels / MIN_UNIT_SIZE;
    if (standardBearers > maxStandardBearers || bellRingers > maxBellRingers)
    {
        return false;
    }

    m_clanshields = clanshields;
    m_numStandardBearers = standardBearers;
    m_numBellRingers = bellRingers;

    // Standard Bearers
    m_retreatAndCharge = (standardBearers > 0);

    Model leader(BASESIZE, WOUNDS);
    if (weapons == RustySpear)
    {
        leader.addMeleeWeapon(&m_rustySpearLeader);
    }
    else if (weapons == RustyBlade)
    {
        leader.addMeleeWeapon(&m_rustyBladeLeader);
    }
    addModel(leader);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == RustySpear)
        {
            model.addMeleeWeapon(&m_rustySpear);
        }
        else if (weapons == RustyBlade)
        {
            model.addMeleeWeapon(&m_rustyBlade);
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

void Clanrats::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_rustySpear);
    visitor(&m_rustySpearLeader);
    visitor(&m_rustyBlade);
    visitor(&m_rustyBladeLeader);
}

Unit *Clanrats::Create(const ParameterList &parameters)
{
    auto unit = new Clanrats();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapon = (WeaponOptions)GetIntParam("weapons", parameters, (int)RustySpear);
    bool clanshields = GetBoolParam("clanshields", parameters, false);
    int standardBearers = GetIntParam("standardBearers", parameters, 0);
    int bellRingers = GetIntParam("bellRingers", parameters, 0);

    bool ok = unit->configure(numModels, weapon, clanshields, standardBearers, bellRingers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Clanrats::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Clanrats", factoryMethod);
    }
}

std::string Clanrats::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == RustySpear)
        {
            return "RustySpear";
        }
        else if (parameter.m_intValue == RustyBlade)
        {
            return "RustyBlade";
        }
    }

    return ParameterValueToString(parameter);
}

int Clanrats::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "RustySpear")
    {
        return RustySpear;
    }
    else if (enumString == "RustyBlade")
    {
        return RustyBlade;
    }
    return 0;
}

int Clanrats::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);
    // Clanshields
    if (m_clanshields && remainingModels() >= 10)
        modifier += 1;
    return modifier;
}

int Clanrats::runModifier() const
{
    int modifier = Unit::runModifier();
    // Bell-ringers
    if (m_numBellRingers > 0)
        modifier += 2;

    return modifier;
}

} // namespace Skaven
