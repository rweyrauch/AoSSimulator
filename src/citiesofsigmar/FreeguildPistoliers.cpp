/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildPistoliers.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildPistoliers::Create,
    FreeguildPistoliers::ValueToString,
    FreeguildPistoliers::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildPistoliers::s_registered = false;

Unit *FreeguildPistoliers::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildPistoliers();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool trumpeter = GetBoolParam("Trumpeter", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, trumpeter);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FreeguildPistoliers::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildPistoliers::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildPistoliers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild Pistoliers", factoryMethod);
    }
}

FreeguildPistoliers::FreeguildPistoliers() :
    CitizenOfSigmar("Freeguild Pistoliers", 12, WOUNDS, 5, 5, false),
    m_pistols(Weapon::Type::Missile, "Brace of Pistols", 9, 2, 4, 3, -1, 1),
    m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
    m_sabreAndPistolButt(Weapon::Type::Melee, "Sabre and Pistol Butt", 1, 2, 4, 4, 0, 1),
    m_hooves(Weapon::Type::Melee, "Stamping Hooves", 1, 2, 4, 5, 0, 1),
    m_sabreAndPistolButtOutrider(Weapon::Type::Melee, "Sabre and Pistol Butt", 1, 3, 4, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_PISTOLIERS};
}

bool FreeguildPistoliers::configure(int numModels, bool trumpeter)
{
    return false;
}

void FreeguildPistoliers::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_pistols);
    visitor(m_handgun);
    visitor(m_sabreAndPistolButt);
    visitor(m_hooves);
    visitor(m_sabreAndPistolButtOutrider);
}

} // namespace CitiesOfSigmar