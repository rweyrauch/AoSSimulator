/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/WildRiders.h"

namespace CitiesOfSigmar
{

bool WildRiders::s_registered = false;

Unit *WildRiders::Create(const ParameterList &parameters)
{
    auto unit = new WildRiders();

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

std::string WildRiders::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int WildRiders::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void WildRiders::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            WildRiders::Create,
            WildRiders::ValueToString,
            WildRiders::EnumStringToInt,
            WildRiders::ComputePoints,
            {
                {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Wild Riders", factoryMethod);
    }
}

WildRiders::WildRiders() :
    CitizenOfSigmar("Wild Riders", 12, WOUNDS, 8, 5, false),
    m_spear(Weapon::Type::Melee, "Hunting Spear", 2, 2, 3, 4, -1, 1),
    m_hooves(Weapon::Type::Melee, "Antlers and Hooves", 1, 2, 4, 4, 0, 1),
    m_spearHunter(Weapon::Type::Melee, "Hunting Spear", 2, 3, 3, 4, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, WILD_RIDERS};
    m_weapons = {&m_spear, &m_hooves, &m_spearHunter};
}

bool WildRiders::configure(int numModels, bool standardBearer, bool hornblower)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Hunter
    auto bossModel = new Model(BASESIZE, WOUNDS);
    bossModel->addMeleeWeapon(&m_spearHunter);
    bossModel->addMeleeWeapon(&m_hooves);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int WildRiders::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_hornblower) mod++;
    return mod;
}

int WildRiders::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_hornblower) mod++;
    return mod;
}

int WildRiders::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

Wounds WildRiders::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Unbound Fury
    if (m_charged && (weapon->name() == m_spear.name()))
    {
        return {2, 0};
    }

    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int WildRiders::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Unbound Fury
    if (m_charged && (weapon->name() == m_spear.name()))
    {
        return -2;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

int WildRiders::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace CitiesOfSigmar