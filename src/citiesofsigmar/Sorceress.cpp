/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Sorceress.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Sorceress::Create,
    Sorceress::ValueToString,
    Sorceress::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Sorceress::s_registered = false;

Unit *Sorceress::Create(const ParameterList &parameters)
{
    auto unit = new Sorceress();

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

std::string Sorceress::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Sorceress::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Sorceress::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sorceress", factoryMethod);
    }
}

Sorceress::Sorceress() :
    CitizenOfSigmar("Sorceress", 6, WOUNDS, 7, 6, false),
    m_witchstaff(Weapon::Type::Melee, "Witchstaff", 2, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, HERO, WIZARD, SORCERESS};
}

bool Sorceress::configure()
{
    return false;
}

void Sorceress::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_witchstaff);
}

} // namespace CitiesOfSigmar