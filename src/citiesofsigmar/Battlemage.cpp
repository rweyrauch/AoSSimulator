/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Battlemage.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Battlemage::Create,
    Battlemage::ValueToString,
    Battlemage::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Battlemage::s_registered = false;

Unit *Battlemage::Create(const ParameterList &parameters)
{
    auto unit = new Battlemage();

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

std::string Battlemage::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Battlemage::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Battlemage::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Battlemage", factoryMethod);
    }

}

Battlemage::Battlemage() :
    CitizenOfSigmar("Battlemage", 5, WOUNDS, 6, 6, false),
    m_staff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, HERO, WIZARD, BATTLEMAGE};
}

bool Battlemage::configure()
{
    return false;
}

void Battlemage::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

}//namespace CitiesOfSigmar