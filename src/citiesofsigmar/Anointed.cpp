/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Anointed.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Anointed::Create,
    Anointed::ValueToString,
    Anointed::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Anointed::s_registered = false;

Unit *Anointed::Create(const ParameterList &parameters)
{
    auto unit = new Anointed();

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Anointed::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Anointed::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Anointed::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Anointed", factoryMethod);
    }
}

Anointed::Anointed() :
    CitizenOfSigmar("Anointed", 6, WOUNDS, 7, 4, false),
    m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, HERO, ANOINTED};
}

bool Anointed::configure()
{
    return false;
}

void Anointed::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_halberd);
}

} //namespace CitiesOfSigmar