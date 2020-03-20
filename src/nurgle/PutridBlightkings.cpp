/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/PutridBlightkings.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Nurgle
{
static const int BASESIZE = 40;
static const int WOUNDS = 4;
static const int MIN_UNIT_SIZE = 5;
static const int MAX_UNIT_SIZE = 20;
static const int POINTS_PER_BLOCK = 160;
static const int POINTS_MAX_UNIT_SIZE = 580;

bool PutridBlightkings::s_registered = false;

PutridBlightkings::PutridBlightkings() :
    NurgleBase("Putrid Blightkings", 4, WOUNDS, 8, 4, false),
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
        auto model = new Model(BASESIZE, wounds());
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
        static FactoryMethod factoryMethod = {
            PutridBlightkings::Create,
            NurgleBase::ValueToString,
            NurgleBase::EnumStringToInt,
            PutridBlightkings::ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Sonorous Tocsin", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
            },
            CHAOS,
            { NURGLE }
        };
        s_registered = UnitFactory::Register("Putrid Blightkings", factoryMethod);
    }
}

int PutridBlightkings::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Blighted Weapons
    if (unmodifiedHitRoll == 6)
    {
        // each 6 inflicts D6 hits
        return Dice::rollD6();
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

void PutridBlightkings::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    if (owningPlayer() == player)
    {
        auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0f);

        // Virulent Discharge
        for (auto unit : units)
        {
            if (Dice::rollD6() >= 6)
            {
                if (unit->hasKeyword(NURGLE)) unit->heal(Dice::rollD3());
                else unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }

}

} // namespace Nurgle

