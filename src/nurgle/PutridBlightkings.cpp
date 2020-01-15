/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/PutridBlightkings.h>
#include <UnitFactory.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    PutridBlightkings::Create,
    nullptr,
    nullptr,
    PutridBlightkings::ComputePoints,
    {
        {
            ParamType::Integer, "Models", PutridBlightkings::MIN_UNIT_SIZE, PutridBlightkings::MIN_UNIT_SIZE,
            PutridBlightkings::MAX_UNIT_SIZE, PutridBlightkings::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Sonorous Tocsin", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    CHAOS,
    { NURGLE }
};

bool PutridBlightkings::s_registered = false;

PutridBlightkings::PutridBlightkings() :
    Unit("Putrid Blightkings", 4, WOUNDS, 8, 4, false),
    m_blightedWeapon(Weapon::Type::Melee, "Blighted Weapon", 1, 3, 3, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, PUTRID_BLIGHTKINGS};
    m_weapons = {&m_blightedWeapon};
}

bool PutridBlightkings::configure(int numModels, bool iconBearer, bool sonorousTocsin)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_sonorousTocsin = sonorousTocsin;

    // Add the Blightlord
    auto leader = new Model(BASESIZE, WOUNDS + 1);
    leader->addMeleeWeapon(&m_blightedWeapon);
    addModel(leader);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_blightedWeapon);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *PutridBlightkings::Create(const ParameterList &parameters)
{
    auto unit = new PutridBlightkings();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool sonorousTocsin = GetBoolParam("Sonorous Tocsin", parameters, false);

    bool ok = unit->configure(numModels, iconBearer, sonorousTocsin);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PutridBlightkings::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Putrid Blightkings", factoryMethod);
    }
}

int PutridBlightkings::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Blighted Weapons
    if (unmodifiedHitRoll == 6)
    {
        Dice dice;
        // each 6 inflicts D6 hits
        return dice.rollD6();
    }

    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

int PutridBlightkings::runModifier() const
{
    int modifier = Unit::runModifier();
    if (m_sonorousTocsin)
    {
        modifier += 1;
    }
    return modifier;
}

int PutridBlightkings::chargeModifier() const
{
    int modifier = Unit::chargeModifier();
    if (m_sonorousTocsin)
    {
        modifier += 1;
    }
    return modifier;
}

int PutridBlightkings::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Nurgle

