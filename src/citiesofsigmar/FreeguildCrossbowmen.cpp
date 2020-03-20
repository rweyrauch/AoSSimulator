/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/FreeguildCrossbowmen.h"

namespace CitiesOfSigmar
{

bool FreeguildCrossbowmen::s_registered = false;

Unit *FreeguildCrossbowmen::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildCrossbowmen();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool piper = GetBoolParam("Piper", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, piper);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FreeguildCrossbowmen::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildCrossbowmen::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildCrossbowmen::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            FreeguildCrossbowmen::Create,
            FreeguildCrossbowmen::ValueToString,
            FreeguildCrossbowmen::EnumStringToInt,
            FreeguildCrossbowmen::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", FreeguildCrossbowmen::MIN_UNIT_SIZE, FreeguildCrossbowmen::MIN_UNIT_SIZE,
                    FreeguildCrossbowmen::MAX_UNIT_SIZE, FreeguildCrossbowmen::MIN_UNIT_SIZE
                },
                {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Piper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Freeguild Crossbowmen", factoryMethod);
    }
}

FreeguildCrossbowmen::FreeguildCrossbowmen() :
    CitizenOfSigmar("Freeguild Crossbowmen", 5, WOUNDS, 5, 6, false),
    m_crossbow(Weapon::Type::Missile, "Freeguild Crossbow", 24, 1, 4, 3, 0, 1),
    m_dagger(Weapon::Type::Melee, "Dagger", 1, 1, 5, 5, 0, 1),
    m_crossbowMarksman(Weapon::Type::Missile, "Freeguild Crossbow", 24, 1, 3, 3, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_CROSSBOWMEN};
    m_weapons = {&m_crossbow, &m_dagger, &m_crossbowMarksman};
}

bool FreeguildCrossbowmen::configure(int numModels, bool standardBearer, bool piper)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_piper = piper;

    // Add the Marksman
    auto bossModel = new Model(BASESIZE, wounds());
    bossModel->addMissileWeapon(&m_crossbowMarksman);
    bossModel->addMeleeWeapon(&m_dagger);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_crossbow);
        model->addMeleeWeapon(&m_dagger);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int FreeguildCrossbowmen::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_piper) mod++;
    return mod;
}

int FreeguildCrossbowmen::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_piper) mod++;
    return mod;
}

int FreeguildCrossbowmen::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

int FreeguildCrossbowmen::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extras = Unit::extraAttacks(attackingModel, weapon, target);
    // Reload, Fire!
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    if (!m_moved && units.empty())
    {
        extras++;
    }
    return extras;
}

int FreeguildCrossbowmen::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace CitiesOfSigmar