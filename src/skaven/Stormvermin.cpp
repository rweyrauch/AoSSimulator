/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Stormvermin.h>
#include <UnitFactory.h>

namespace Skaven
{
static FactoryMethod factoryMethod = {
    Stormvermin::Create,
    nullptr,
    nullptr,
    Stormvermin::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Stormvermin::MIN_UNIT_SIZE, Stormvermin::MIN_UNIT_SIZE,
            Stormvermin::MAX_UNIT_SIZE, Stormvermin::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Clanshields", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Integer, "Standard Bearers", 0, 0, Stormvermin::MAX_UNIT_SIZE / Stormvermin::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Drummers", 0, 0, Stormvermin::MAX_UNIT_SIZE / Stormvermin::MIN_UNIT_SIZE, 1}
    },
    CHAOS,
    { SKAVEN }
};

bool Stormvermin::s_registered = false;

Stormvermin::Stormvermin() :
    Skaventide("Stormvermin", 6, WOUNDS, 5, 5, false),
    m_rustyHalberd(Weapon::Type::Melee, "Rusty Halberd", 2, 2, 4, 3, -1, 1),
    m_rustyHalberdLeader(Weapon::Type::Melee, "Rusty Halberd", 2, 3, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, STORMVERMIN};
    m_weapons = {&m_rustyHalberd, &m_rustyHalberdLeader};
}

bool Stormvermin::configure(int numModels, bool clanshields, int standardBearers, int drummers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    int maxStandardBearers = numModels / MIN_UNIT_SIZE;
    int maxDummers = numModels / MIN_UNIT_SIZE;
    if (standardBearers > maxStandardBearers || drummers > maxDummers)
    {
        return false;
    }

    m_clanshields = clanshields;
    m_numStandardBearers = standardBearers;
    m_numDrummers = drummers;

    // Standard Bearers
    m_retreatAndCharge = (standardBearers > 0);

    auto leader = new Model(BASESIZE, WOUNDS);
    leader->addMeleeWeapon(&m_rustyHalberdLeader);
    addModel(leader);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_rustyHalberd);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Stormvermin::Create(const ParameterList &parameters)
{
    auto unit = new Stormvermin();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool clanshields = GetBoolParam("Clanshields", parameters, false);
    int standardBearers = GetIntParam("Standard Bearers", parameters, 0);
    int drummers = GetIntParam("Drummers", parameters, 0);

    bool ok = unit->configure(numModels, clanshields, standardBearers, drummers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Stormvermin::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Stormvermin", factoryMethod);
    }
}

int Stormvermin::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Skaventide::toSaveModifier(weapon);
    // Clanshields
    if (m_clanshields && remainingModels() >= 10)
        modifier += 1;
    return modifier;
}

int Stormvermin::runModifier() const
{
    int modifier = Skaventide::runModifier();
    // Drummer
    if (m_numDrummers > 0)
        modifier += 2;
    return modifier;
}

int Stormvermin::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Skaven
