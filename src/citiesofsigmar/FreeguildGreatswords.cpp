/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/FreeguildGreatswords.h"

namespace CitiesOfSigmar
{
static const int BASESIZE = 25;
static const int WOUNDS = 1;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 30;
static const int POINTS_PER_BLOCK = 160;
static const int POINTS_MAX_UNIT_SIZE = 420;

bool FreeguildGreatswords::s_registered = false;

Unit *FreeguildGreatswords::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildGreatswords();

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

std::string FreeguildGreatswords::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildGreatswords::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildGreatswords::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            FreeguildGreatswords::Create,
            FreeguildGreatswords::ValueToString,
            FreeguildGreatswords::EnumStringToInt,
            FreeguildGreatswords::ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Freeguild Greatswords", factoryMethod);
    }
}

FreeguildGreatswords::FreeguildGreatswords() :
    CitizenOfSigmar("Freeguild Greatswords", 5, WOUNDS, 6, 4, false),
    m_zweihander(Weapon::Type::Melee, "Zweihander", 1, 2, 3, 3, -1, 1),
    m_zweihanderChampion(Weapon::Type::Melee, "Zweihander", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_GREATSWORDS};
    m_weapons = {&m_zweihander, &m_zweihanderChampion};
}

bool FreeguildGreatswords::configure(int numModels, bool standardBearer, bool hornblower)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Champion
    auto bossModel = new Model(BASESIZE, wounds());
    bossModel->addMeleeWeapon(&m_zweihanderChampion);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_zweihander);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int FreeguildGreatswords::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_hornblower) mod++;
    return mod;
}

int FreeguildGreatswords::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_hornblower) mod++;
    return mod;
}

int FreeguildGreatswords::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

Wounds FreeguildGreatswords::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Decapitating Swing
    auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    if (hitRoll == 6) damage.mortal++;
    return damage;
}

int FreeguildGreatswords::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);
    // Oathsworn Honour Guard
    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 18.0f);
    if (unit && unit->hasKeyword(FREEGUILD)) mod++;
    return mod;
}

int FreeguildGreatswords::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace CitiesOfSigmar