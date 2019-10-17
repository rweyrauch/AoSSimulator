/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FrostheartPhoenix.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FrostheartPhoenix::Create,
    FrostheartPhoenix::ValueToString,
    FrostheartPhoenix::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FrostheartPhoenix::s_registered = false;

Unit *FrostheartPhoenix::Create(const ParameterList &parameters)
{
    auto unit = new FrostheartPhoenix();

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

std::string FrostheartPhoenix::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FrostheartPhoenix::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FrostheartPhoenix::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Frostheart Phoenix", factoryMethod);
    }
}

FrostheartPhoenix::FrostheartPhoenix()
{

}

bool FrostheartPhoenix::configure()
{
    return false;
}

void FrostheartPhoenix::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar