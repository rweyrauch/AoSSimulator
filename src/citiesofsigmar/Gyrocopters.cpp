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

std::string Gyrocopters::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Gyrocopters::EnumStringToInt(const std::string &enumString)
{
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
}

bool Gyrocopters::configure(int numModels)
{
    return false;
}

void Gyrocopters::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar