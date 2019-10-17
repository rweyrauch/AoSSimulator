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

    auto anointed = GetBoolParam("Anointed", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(anointed);
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

FrostheartPhoenix::FrostheartPhoenix() :
    CitizenOfSigmar("Frostheart Phoenix", 16, WOUNDS, 9, 4, true),
    m_talons(Weapon::Type::Melee, "Ice-cold Talons", 2, 8, 3, 3, -1, 2),
    m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, MONSTER, FROSTHEART_PHOENIX};
}

bool FrostheartPhoenix::configure(bool anointed)
{
    if (anointed)
    {
        addKeyword(HERO);
    }

    return false;
}

void FrostheartPhoenix::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_talons);
    visitor(m_halberd);
}

} // namespace CitiesOfSigmar