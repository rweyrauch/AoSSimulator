/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Executioners.h>
#include <UnitFactory.h>

#include "citiesofsigmar/EternalGuard.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Executioners::Create,
    Executioners::ValueToString,
    Executioners::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Executioners::s_registered = false;

Unit *Executioners::Create(const ParameterList &parameters)
{
    auto unit = new Executioners();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool drummer = GetBoolParam("Drummer", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Executioners::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Executioners::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Executioners::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Executioners", factoryMethod);
    }
}

Executioners::Executioners() :
    CitizenOfSigmar("Executioners", 6, WOUNDS, 7, 4, false),
    m_draich(Weapon::Type::Melee, "Executioner's Draich", 1, 2, 3, 3, 0, 1),
    m_draichMaster(Weapon::Type::Melee, "Executioner's Draich", 1, 3, 3, 3, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, EXECUTIONERS};
}

bool Executioners::configure(int numModels, bool standardBearer, bool drummer)
{
    return false;
}

void Executioners::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_draich);
    visitor(m_draichMaster);
}

} // namespace CitiesOfSigmar