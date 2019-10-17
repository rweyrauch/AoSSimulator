/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/SteamTank.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    SteamTank::Create,
    SteamTank::ValueToString,
    SteamTank::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool SteamTank::s_registered = false;

Unit *SteamTank::Create(const ParameterList &parameters)
{
    auto unit = new SteamTank();

    auto commander = GetBoolParam("Commander", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(commander);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string SteamTank::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int SteamTank::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void SteamTank::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Steam Tank", factoryMethod);
    }
}

SteamTank::SteamTank() :
    CitizenOfSigmar("Steam Tank", RAND_2D6, WOUNDS, 8, 3, false),
    m_steamCannon(Weapon::Type::Missile, "Steam Cannon", 30, 1, 4, 2, -2, RAND_D6),
    m_steamGun(Weapon::Type::Missile, "Steam Gun", 8, RAND_2D6, 4, 2, 0, 1),
    m_longRifle(Weapon::Type::Missile, "Long Rifle", 30, 1, 3, 3, -1, 2),
    m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
    m_crushingWheels(Weapon::Type::Melee, "Crushing Wheels", 1, RAND_D6, 4, 3, -1, 2),
    m_sword(Weapon::Type::Melee, "Sword or Rod", 1, 2, 5, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, STEAM_TANK};
}

bool SteamTank::configure(bool commander)
{
    if (commander)
    {
        addKeyword(HERO);
    }

    return false;
}

void SteamTank::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_steamCannon);
    visitor(m_steamGun);
    visitor(m_longRifle);
    visitor(m_handgun);
    visitor(m_crushingWheels);
    visitor(m_sword);
}

} // namespace CitiesOfSigmar