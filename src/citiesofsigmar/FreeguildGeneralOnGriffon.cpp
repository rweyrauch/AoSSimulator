/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGeneralOnGriffon.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildGeneralOnGriffon::Create,
    FreeguildGeneralOnGriffon::ValueToString,
    FreeguildGeneralOnGriffon::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildGeneralOnGriffon::s_registered = false;

Unit *FreeguildGeneralOnGriffon::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildGeneralOnGriffon();

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

std::string FreeguildGeneralOnGriffon::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildGeneralOnGriffon::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildGeneralOnGriffon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild General on Griffon", factoryMethod);
    }
}

FreeguildGeneralOnGriffon::FreeguildGeneralOnGriffon() :
    CitizenOfSigmar("Freeguild General on Griffon", 15, WOUNDS, 7, 4, true),
    m_runesword(Weapon::Type::Melee, "Sigmarite Runesword", 1, 5, 3, 4, -1, 2),
    m_greathammer(Weapon::Type::Melee, "Sigmarite Greathammer", 1, 3, 3, 3, -2, RAND_D3),
    m_lance(Weapon::Type::Melee, "Freeguild Lance", 2, 4, 3, 4, -1, 2),
    m_claws(Weapon::Type::Melee, "Razor Claws", 2, 6, 4, 3, -1, 2),
    m_beak(Weapon::Type::Melee, "Deadly Beak", 2, 2, 3, 3, -2, 4)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, MONSTER, HERO,FREEGUILD_GENERAL};
}

bool FreeguildGeneralOnGriffon::configure()
{
    return false;
}

void FreeguildGeneralOnGriffon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar