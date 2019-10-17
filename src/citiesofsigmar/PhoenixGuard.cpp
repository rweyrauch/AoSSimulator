/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/PhoenixGuard.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    PhoenixGuard::Create,
    PhoenixGuard::ValueToString,
    PhoenixGuard::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool PhoenixGuard::s_registered = false;

Unit *PhoenixGuard::Create(const ParameterList &parameters)
{
    auto unit = new PhoenixGuard();

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

std::string PhoenixGuard::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int PhoenixGuard::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void PhoenixGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Phoenix Guard", factoryMethod);
    }
}

PhoenixGuard::PhoenixGuard() :
    CitizenOfSigmar("Phoenix Guard", 6, WOUNDS, 7, 4, false),
    m_halberd(Weapon::Type::Melee, "Phoenix Halberd", 2, 2, 3, 3, -1, 1),
    m_halberdKeeper(Weapon::Type::Melee, "Phoenix Halberd", 2, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, PHOENIX_GUARD};
}

bool PhoenixGuard::configure(int numModels, bool standardBearer, bool drummer)
{
    return false;
}

void PhoenixGuard::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar