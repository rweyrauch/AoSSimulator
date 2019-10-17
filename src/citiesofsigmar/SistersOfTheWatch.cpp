/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/SistersOfTheWatch.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    SistersOfTheWatch::Create,
    SistersOfTheWatch::ValueToString,
    SistersOfTheWatch::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool SistersOfTheWatch::s_registered = false;

Unit *SistersOfTheWatch::Create(const ParameterList &parameters)
{
    auto unit = new SistersOfTheWatch();

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

std::string SistersOfTheWatch::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int SistersOfTheWatch::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void SistersOfTheWatch::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sisters of the Watch", factoryMethod);
    }
}

SistersOfTheWatch::SistersOfTheWatch() :
    CitizenOfSigmar("Sisters of the Watch", 6, WOUNDS, 7, 5, false),
    m_bow(Weapon::Type::Missile, "Watch Bow", 18, 1, 3, 3, 0, 1),
    m_sword(Weapon::Type::Melee, "Ithilmar Sword", 1, 1, 4, 4, 0, 1),
    m_bowHighSister(Weapon::Type::Missile, "Watch Bow", 18, 2, 3, 3, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, SISTERS_OF_THE_WATCH};
}

bool SistersOfTheWatch::configure(int numModels)
{
    return false;
}

void SistersOfTheWatch::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar