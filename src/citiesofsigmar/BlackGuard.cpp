/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/BlackGuard.h"

namespace CitiesOfSigmar
{

bool BlackGuard::s_registered = false;

Unit *BlackGuard::Create(const ParameterList &parameters)
{
    auto unit = new BlackGuard();

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool drummer = GetBoolParam("Drummer", parameters, true);

    bool ok = unit->configure(numModels, standard, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string BlackGuard::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int BlackGuard::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void BlackGuard::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            BlackGuard::Create,
            BlackGuard::ValueToString,
            BlackGuard::EnumStringToInt,
            BlackGuard::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", BlackGuard::MIN_UNIT_SIZE, BlackGuard::MIN_UNIT_SIZE,
                    BlackGuard::MAX_UNIT_SIZE, BlackGuard::MIN_UNIT_SIZE
                },
                {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Drummer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Black Guard", factoryMethod);
    }
}

BlackGuard::BlackGuard() :
    CitizenOfSigmar("Black Guard", 6, WOUNDS, 8, 4, false),
    m_halberd(Weapon::Type::Melee, "Ebon Halberd", 2, 2, 3, 3, -1, 1),
    m_halberdCaptain(Weapon::Type::Melee, "Ebon Halberd", 2, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, BLACK_GUARD};
    m_weapons = {&m_halberd, &m_halberdCaptain};
}

bool BlackGuard::configure(int numModels, bool standardBearer, bool drummer)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_drummer = drummer;

    // Add the Captain
    auto bossModel = new Model(BASESIZE, WOUNDS);
    bossModel->addMeleeWeapon(&m_halberdCaptain);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_halberd);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int BlackGuard::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_drummer) mod++;
    return mod;
}

int BlackGuard::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_drummer) mod++;
    return mod;
}

int BlackGuard::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

int BlackGuard::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);
    // Elite Bodyguard
    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 18.0f);
    if (unit && unit->hasKeyword(DARKLING_COVENS)) mod++;
    return mod;
}

int BlackGuard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace CitiesOfSigmar