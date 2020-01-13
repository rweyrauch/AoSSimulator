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
            ParamType::Integer, "Models", Clanrats::MIN_UNIT_SIZE, Clanrats::MIN_UNIT_SIZE,
            Clanrats::MAX_UNIT_SIZE, Clanrats::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", Clanrats::RustySpear, Clanrats::RustySpear,
            Clanrats::RustyBlade, 1
        },
        {ParamType::Boolean, "Clanshields", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Integer, "Standard Bearers", 0, 0, Clanrats::MAX_UNIT_SIZE/Clanrats::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Bell Ringers", 0, 0, Clanrats::MAX_UNIT_SIZE/Clanrats::MIN_UNIT_SIZE, 1}
    },
    CHAOS,
    { SKAVEN }
};

bool Clanrats::s_registered = false;

Clanrats::Clanrats() :
    Skaventide("Clanrats", 6, WOUNDS, 4, 6, false),
    m_rustySpear(Weapon::Type::Melee, "Rusty Spear", 2, 1, 5, 4, 0, 1),
    m_rustySpearLeader(Weapon::Type::Melee, "Rusty Spear", 2, 2, 5, 4, 0, 1),
    m_rustyBlade(Weapon::Type::Melee, "Rusty Blade", 1, 1, 4, 4, 0, 1),
    m_rustyBladeLeader(Weapon::Type::Melee, "Rusty Blade", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, CLANRATS};
    m_weapons = {&m_rustySpear, &m_rustySpearLeader, &m_rustyBlade, &m_rustyBladeLeader};
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

    auto leader = new Model(BASESIZE, WOUNDS);
    if (weapons == RustySpear)
    {
        leader->addMeleeWeapon(&m_rustySpearLeader);
    }
    else if (weapons == RustyBlade)
    {
        leader->addMeleeWeapon(&m_rustyBladeLeader);
    }
    addModel(leader);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == RustySpear)
        {
            model->addMeleeWeapon(&m_rustySpear);
        }
        else if (weapons == RustyBlade)
        {
            model->addMeleeWeapon(&m_rustyBlade);
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

Unit *Clanrats::Create(const ParameterList &parameters)
{
    auto unit = new Clanrats();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapon = (WeaponOptions)GetEnumParam("Weapons", parameters, (int)RustySpear);
    bool clanshields = GetBoolParam("Clanshields", parameters, false);
    int standardBearers = GetIntParam("Standard Bearers", parameters, 0);
    int bellRingers = GetIntParam("Bell Ringers", parameters, 0);

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
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == RustySpear)
        {
            return "Rusty Spear";
        }
        else if (parameter.intValue == RustyBlade)
        {
            return "Rusty Blade";
        }
    }

    return ParameterValueToString(parameter);
}

int Clanrats::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Rusty Spear")
    {
        return RustySpear;
    }
    else if (enumString == "Rusty Blade")
    {
        return RustyBlade;
    }
    return 0;
}

int Clanrats::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Skaventide::toSaveModifier(weapon);
    // Clanshields
    if (m_clanshields && remainingModels() >= 10)
        modifier += 1;
    return modifier;
}

int Clanrats::runModifier() const
{
    int modifier = Skaventide::runModifier();
    // Bell-ringers
    if (m_numBellRingers > 0)
        modifier += 2;

    return modifier;
}

} // namespace Skaven
