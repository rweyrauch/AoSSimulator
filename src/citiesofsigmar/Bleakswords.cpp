/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Bleakswords.h"

namespace CitiesOfSigmar
{
static const int BASESIZE = 32;
static const int WOUNDS = 1;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 40;
static const int POINTS_PER_BLOCK = 90;
static const int POINTS_MAX_UNIT_SIZE = 320;

bool Bleakswords::s_registered = false;

Unit *Bleakswords::Create(const ParameterList &parameters)
{
    auto unit = new Bleakswords();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool hornblower = GetBoolParam("Hornblower", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Bleakswords::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Bleakswords::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Bleakswords::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Bleakswords::Create,
            Bleakswords::ValueToString,
            Bleakswords::EnumStringToInt,
            Bleakswords::ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Bleakswords", factoryMethod);
    }
}

Bleakswords::Bleakswords() :
    CitizenOfSigmar("Bleakswords", 6, WOUNDS, 6, 4, false),
    m_sword(Weapon::Type::Melee, "Darkling Sword", 1, 1, 4, 4, 0, 1),
    m_swordLordling(Weapon::Type::Melee, "Darkling Sword", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, BLEAKSWORDS};
    m_weapons = {&m_sword, &m_swordLordling};
}

bool Bleakswords::configure(int numModels, bool standardBearer, bool hornblower)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Lordling
    auto bossModel = new Model(BASESIZE, wounds());
    bossModel->addMeleeWeapon(&m_swordLordling);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_sword);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int Bleakswords::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_hornblower) mod++;
    return mod;
}

int Bleakswords::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_hornblower) mod++;
    return mod;
}

int Bleakswords::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

int Bleakswords::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Quicksilver Strike
    if (unmodifiedHitRoll == 6)
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

int Bleakswords::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);
    // Ranks of Cold Steel
    if (remainingModels() >= 10) mod++;
    return mod;
}

int Bleakswords::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace CitiesOfSigmar