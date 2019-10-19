/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Darkshards.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Darkshards::Create,
    Darkshards::ValueToString,
    Darkshards::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Darkshards::MIN_UNIT_SIZE, Darkshards::MIN_UNIT_SIZE,
            Darkshards::MAX_UNIT_SIZE, Darkshards::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Darkshards::s_registered = false;

Unit *Darkshards::Create(const ParameterList &parameters)
{
    auto unit = new Darkshards();

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

std::string Darkshards::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Darkshards::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Darkshards::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Darkshards", factoryMethod);
    }
}

Darkshards::Darkshards() :
    CitizenOfSigmar("Darkshards", 6, WOUNDS, 6, 5, false),
    m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 2, 4, 4, 0, 1),
    m_dagger(Weapon::Type::Melee, "Cruel Dagger", 1, 1, 5, 5, 0, 1),
    m_crossbowMaster(Weapon::Type::Missile, "Repeater Crossbow", 16, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, DARKSHARDS};
}

bool Darkshards::configure(int numModels, bool standardBearer, bool hornblower)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Guardmaster
    Model bossModel(BASESIZE, WOUNDS);
    bossModel.addMissileWeapon(&m_crossbowMaster);
    bossModel.addMeleeWeapon(&m_dagger);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_crossbow);
        model.addMeleeWeapon(&m_dagger);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Darkshards::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_crossbow);
    visitor(m_dagger);
    visitor(m_crossbowMaster);
}

} //namespace CitiesOfSigmar