/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Kharibdyss.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Kharibdyss::Create,
    Kharibdyss::ValueToString,
    Kharibdyss::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Kharibdyss::s_registered = false;

Unit *Kharibdyss::Create(const ParameterList &parameters)
{
    auto unit = new Kharibdyss();

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

std::string Kharibdyss::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Kharibdyss::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Kharibdyss::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kharibdyss", factoryMethod);
    }
}

Kharibdyss::Kharibdyss() :
    CitizenOfSigmar("Kharibdyss", 7, WOUNDS, 6, 4, false),
    m_tentacles(Weapon::Type::Melee, "Fanged Tentacles", 3, 6, 4, 3, -1, 2),
    m_tail(Weapon::Type::Melee, "Spiked Tail", 2, RAND_D6, 4, 2, 0, 1),
    m_limbs(Weapon::Type::Melee, "Clawed Limbs", 1, 2, 3, 3, -1, 1),
    m_goadsAndWhips(Weapon::Type::Melee, "Cruel Goads and Whips", 2, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, MONSTER, KHARIBDYSS};
}

bool Kharibdyss::configure()
{
    return false;
}

void Kharibdyss::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar