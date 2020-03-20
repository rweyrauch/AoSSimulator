/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/Flagellants.h"

namespace CitiesOfSigmar
{
static const int BASESIZE = 25;
static const int WOUNDS = 1;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 40;
static const int POINTS_PER_BLOCK = 80;
static const int POINTS_MAX_UNIT_SIZE = 280;

bool Flagellants::s_registered = false;

Unit *Flagellants::Create(const ParameterList &parameters)
{
    auto unit = new Flagellants();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Flagellants::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Flagellants::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Flagellants::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Flagellants::Create,
            Flagellants::ValueToString,
            Flagellants::EnumStringToInt,
            Flagellants::ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Flagellants", factoryMethod);
    }
}

Flagellants::Flagellants() :
    CitizenOfSigmar("Flagellants", 6, WOUNDS, 8, NoSave, false),
    m_flailsAndClubs(Weapon::Type::Melee, "Castigating Flails and Clubs", 1, 2, 5, 4, 0, 1),
    m_flailsAndClubsProphet(Weapon::Type::Melee, "Castigating Flails and Clubs", 1, 3, 5, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, DEVOTED_OF_SIGMAR, FLAGELLANTS};
    m_weapons = {&m_flailsAndClubs, &m_flailsAndClubsProphet};
}

bool Flagellants::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Prophet
    auto bossModel = new Model(BASESIZE, wounds());
    bossModel->addMeleeWeapon(&m_flailsAndClubsProphet);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_flailsAndClubs);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int Flagellants::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extras = Unit::extraAttacks(attackingModel, weapon, target);

    // Glorious Martyrs
    if (m_modelsSlain >= 5)
    {
        extras += 2;
    }
    else if (m_modelsSlain)
    {
        extras++;
    }
    return extras;
}

int Flagellants::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Fanatical Fury
    if (m_charged) mod++;

    return mod;
}

int Flagellants::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toWoundModifier(weapon, target);

    // Fanatical Fury
    if (m_charged) mod++;

    return mod;
}

void Flagellants::onFlee(int numFled)
{
    // Reckless Abandon
    Dice::RollResult rolls;
    Dice::rollD6(numFled, rolls);
    int numMortalWounds = rolls.rollsGE(4);
    if (numMortalWounds)
    {
        auto board = Board::Instance();
        auto closestTarget = board->getNearestUnit(this, PlayerId::None);
        if (closestTarget && distanceTo(closestTarget) <= 6.0f)
        {
            closestTarget->applyDamage({0, numMortalWounds});
        }
    }
    Unit::onFlee(numFled);
}

int Flagellants::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace CitiesOfSigmar