/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildOutriders.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildOutriders::Create,
    FreeguildOutriders::ValueToString,
    FreeguildOutriders::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildOutriders::s_registered = false;

Unit *FreeguildOutriders::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildOutriders();

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

std::string FreeguildOutriders::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildOutriders::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildOutriders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild Outriders", factoryMethod);
    }
}

FreeguildOutriders::FreeguildOutriders() :
    CitizenOfSigmar("Freeguild Outriders", 12, WOUNDS, 6, 5, false),
    m_blunderbuss(Weapon::Type::Missile, "Grenade-launching Blunderbuss", 12, 1, 4, 3, -1, RAND_D3),
    m_pistols(Weapon::Type::Missile, "Brace of Pistols", 9, 2, 3, 3, -1, 1),
    m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 5, 3, -1, 1),
    m_sabre(Weapon::Type::Melee, "Freeguild Cavalry Sabre", 1, 1, 4, 4, 0, 1),
    m_hooves(Weapon::Type::Melee, "Stamping Hooves", 1, 2, 4, 5, 0, 1),
    m_sabreSharpshooter(Weapon::Type::Melee, "Freeguild Cavalry Sabre", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_OUTRIDERS};
}

bool FreeguildOutriders::configure(int numModels, bool trumpeter)
{
    return false;
}

void FreeguildOutriders::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar