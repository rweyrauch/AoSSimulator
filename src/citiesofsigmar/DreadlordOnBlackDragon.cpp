/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DreadlordOnBlackDragon.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    DreadlordOnBlackDragon::Create,
    DreadlordOnBlackDragon::ValueToString,
    DreadlordOnBlackDragon::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool DreadlordOnBlackDragon::s_registered = false;

Unit *DreadlordOnBlackDragon::Create(const ParameterList &parameters)
{
    auto unit = new DreadlordOnBlackDragon();

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

std::string DreadlordOnBlackDragon::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int DreadlordOnBlackDragon::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void DreadlordOnBlackDragon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dreadlord on Black Dragon", factoryMethod);
    }
}

DreadlordOnBlackDragon::DreadlordOnBlackDragon() :
    CitizenOfSigmar("Dreadlord on Black Dragon", 12, WOUNDS, 8, 4, true),
    m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 4, 4, 0, 1),
    m_noxiousBreath(Weapon::Type::Missile, "Noxious Breath", 6, 1, 0, 0, 0, 0),
    m_blade(Weapon::Type::Melee, "Exile Blade", 1, 6, 3, 4, 0, 1),
    m_lance(Weapon::Type::Melee, "Lance of Spite", 2, 3, 3, 3, -1, 1),
    m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 3, 3, 4, 2, -2, RAND_D6),
    m_claws(Weapon::Type::Melee, "Razor-sharp Claws", 2, 6, 4, 3, -1, 2)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, MONSTER, HERO, DREADLORD};
}

bool DreadlordOnBlackDragon::configure()
{
    return false;
}

void DreadlordOnBlackDragon::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_crossbow);
    visitor(m_noxiousBreath);
    visitor(m_blade);
    visitor(m_lance);
    visitor(m_jaws);
    visitor(m_claws);
}

} // namespace CitiesOfSigmar