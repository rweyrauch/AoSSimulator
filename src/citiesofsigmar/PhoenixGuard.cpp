/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/PhoenixGuard.h"

namespace CitiesOfSigmar
{

bool PhoenixGuard::s_registered = false;

Unit *PhoenixGuard::Create(const ParameterList &parameters)
{
    auto unit = new PhoenixGuard();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool drummer = GetBoolParam("Drummer", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string PhoenixGuard::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int PhoenixGuard::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void PhoenixGuard::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            PhoenixGuard::Create,
            PhoenixGuard::ValueToString,
            PhoenixGuard::EnumStringToInt,
            PhoenixGuard::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", PhoenixGuard::MIN_UNIT_SIZE, PhoenixGuard::MIN_UNIT_SIZE,
                    PhoenixGuard::MAX_UNIT_SIZE, PhoenixGuard::MIN_UNIT_SIZE
                },
                {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Drummer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Phoenix Guard", factoryMethod);
    }
}

PhoenixGuard::PhoenixGuard() :
    CitizenOfSigmar("Phoenix Guard", 6, WOUNDS, 7, 4, false),
    m_halberd(Weapon::Type::Melee, "Phoenix Halberd", 2, 2, 3, 3, -1, 1),
    m_halberdKeeper(Weapon::Type::Melee, "Phoenix Halberd", 2, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, PHOENIX_GUARD};
    m_weapons = {&m_halberd, &m_halberdKeeper};
}

bool PhoenixGuard::configure(int numModels, bool standardBearer, bool drummer)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_drummer = drummer;

    // Add the Keeper
    auto bossModel = new Model(BASESIZE, WOUNDS);
    bossModel->addMeleeWeapon(&m_halberdKeeper);
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

int PhoenixGuard::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_drummer) mod++;
    return mod;
}

int PhoenixGuard::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_drummer) mod++;
    return mod;
}

int PhoenixGuard::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

Wounds PhoenixGuard::applyWoundSave(const Wounds &wounds)
{
    // Witness to Destiny
    Dice::RollResult woundSaves, mortalSaves;
    Dice::rollD6(wounds.normal, woundSaves);
    Dice::rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(4);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(4);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

int PhoenixGuard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

bool PhoenixGuard::battleshockRequired() const
{
    // Emboldened
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
    for (auto unit : units)
    {
        if (unit->hasKeyword(PHOENIX_TEMPLE) && unit->hasKeyword(HERO)) return false;
    }
    return Unit::battleshockRequired();
}

} // namespace CitiesOfSigmar