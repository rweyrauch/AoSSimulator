/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Gyrocopters.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Gyrocopters::Create,
    Gyrocopters::ValueToString,
    Gyrocopters::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Gyrocopters::MIN_UNIT_SIZE, Gyrocopters::MIN_UNIT_SIZE,
            Gyrocopters::MAX_UNIT_SIZE, Gyrocopters::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", Gyrocopters::BrimstoneGun, Gyrocopters::BrimstoneGun,
            Gyrocopters::SteamGun, 1
        },
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Gyrocopters::s_registered = false;

Unit *Gyrocopters::Create(const ParameterList &parameters)
{
    auto unit = new Gyrocopters();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, BrimstoneGun);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Gyrocopters::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == BrimstoneGun)
        {
            return "Brimstone Gun";
        }
        else if (parameter.m_intValue == SteamGun)
        {
            return "Steam Gun";
        }
    }
    return CitizenOfSigmar::ValueToString(parameter);
}

int Gyrocopters::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Brimstone Gun")
    {
        return BrimstoneGun;
    }
    else if (enumString == "Steam Gun")
    {
        return SteamGun;
    }
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Gyrocopters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gyrocopters", factoryMethod);
    }
}

Gyrocopters::Gyrocopters() :
    CitizenOfSigmar("Gyrocopters", 16, WOUNDS, 6, 4, true),
    m_brimstoneGun(Weapon::Type::Missile, "Brimstone Gun", 16, 3, 3, 3, -1, 1),
    m_steamGun(Weapon::Type::Missile, "Steam Gun", 8, 1, 3, 4, -1, 1),
    m_rotorBlades(Weapon::Type::Melee, "Rotor Blades", 1, RAND_D3, 5, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, GYROCOPTERS};
    m_weapons = {&m_brimstoneGun, &m_steamGun, &m_rotorBlades};
}

bool Gyrocopters::configure(int numModels, WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == BrimstoneGun)
        {
            model->addMissileWeapon(&m_brimstoneGun);
        }
        else if (weapons == SteamGun)
        {
            model->addMissileWeapon(&m_steamGun);
        }
        model->addMeleeWeapon(&m_rotorBlades);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

} // namespace CitiesOfSigmar