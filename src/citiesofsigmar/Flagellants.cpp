/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Flagellants.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Flagellants::Create,
    Flagellants::ValueToString,
    Flagellants::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Flagellants::s_registered = false;

Unit *Flagellants::Create(const ParameterList &parameters)
{
    auto unit = new Flagellants();

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

std::string Flagellants::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Flagellants::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Flagellants::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Flagellants", factoryMethod);
    }
}

Flagellants::Flagellants() :
    CitizenOfSigmar("Flagellants", 6, WOUNDS, 8, NoSave, false),
    m_flailsAndClubs(Weapon::Type::Melee, "Castigating Flails and Clubs", 1, 2, 5, 4, 0, 1),
    m_flailsAndClubsProphet(Weapon::Type::Melee, "Castigating Flails and Clubs", 1, 3, 5, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, DEVOTED_OF_SIGMAR, FLAGELLANTS};
}

bool Flagellants::configure(int numModels)
{
    return false;
}

void Flagellants::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_flailsAndClubs);
    visitor(m_flailsAndClubsProphet);
}

} // namespace CitiesOfSigmar