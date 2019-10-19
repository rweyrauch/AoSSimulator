/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DarkRiders.h"

namespace CitiesOfSigmar
{

static FactoryMethod factoryMethod = {
    DarkRiders::Create,
    DarkRiders::ValueToString,
    DarkRiders::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", DarkRiders::MIN_UNIT_SIZE, DarkRiders::MIN_UNIT_SIZE,
            DarkRiders::MAX_UNIT_SIZE, DarkRiders::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool DarkRiders::s_registered = false;

Unit *DarkRiders::Create(const ParameterList &parameters)
{
    auto unit = new DarkRiders();

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

std::string DarkRiders::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int DarkRiders::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void DarkRiders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dark Riders", factoryMethod);
    }
}

DarkRiders::DarkRiders() :
    CitizenOfSigmar("Dark Riders", 14, WOUNDS, 6, 4, false),
    m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 3, 5, 4, 0, 1),
    m_spear(Weapon::Type::Melee, "Barbed Spear", 2, 1, 4, 4, 0, 1),
    m_bite(Weapon::Type::Melee, "Vicious Bite", 1, 2, 4, 5, 0, 1),
    m_crossbowHerald(Weapon::Type::Missile, "Repeater Crossbow", 16, 3, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SHADOWBLADES, DARK_RIDERS};
}

bool DarkRiders::configure(int numModels, bool standardBearer, bool hornblower)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Herald
    Model bossModel(BASESIZE, WOUNDS);
    bossModel.addMissileWeapon(&m_crossbowHerald);
    bossModel.addMeleeWeapon(&m_spear);
    bossModel.addMeleeWeapon(&m_bite);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_crossbow);
        model.addMeleeWeapon(&m_spear);
        model.addMeleeWeapon(&m_bite);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void DarkRiders::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_crossbow);
    visitor(m_spear);
    visitor(m_bite);
    visitor(m_crossbowHerald);
}

} //namespace CitiesOfSigmar