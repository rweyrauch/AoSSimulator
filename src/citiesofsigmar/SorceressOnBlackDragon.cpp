/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/SorceressOnBlackDragon.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    SorceressOnBlackDragon::Create,
    SorceressOnBlackDragon::ValueToString,
    SorceressOnBlackDragon::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool SorceressOnBlackDragon::s_registered = false;

Unit *SorceressOnBlackDragon::Create(const ParameterList &parameters)
{
    auto unit = new SorceressOnBlackDragon();

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

std::string SorceressOnBlackDragon::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int SorceressOnBlackDragon::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void SorceressOnBlackDragon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sorceress on Black Dragon", factoryMethod);
    }
}

SorceressOnBlackDragon::SorceressOnBlackDragon() :
    CitizenOfSigmar("Sorceress on Black Dragon", 14, WOUNDS, 7, 5, true),
    m_noxiousBreath(Weapon::Type::Missile, "Noxious Breath", 6, 1, 0, 0, 0, 0),
    m_rod(Weapon::Type::Melee, "Witch Rod", 1, 1, 4, 3, -1, RAND_D3),
    m_sword(Weapon::Type::Melee, "Darkling Sword", 1, 3, 4, 4, 0, 1),
    m_lash(Weapon::Type::Melee, "Witch Lash", 2, 1, 3, 4, 0, 1),
    m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 3, 3, 4, 1, -2, RAND_D6),
    m_claws(Weapon::Type::Melee, "Razor-sharp Claws", 2, 6, 4, 3, -1, 2)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, MONSTER, HERO, WIZARD, SORCERESS};
}

bool SorceressOnBlackDragon::configure()
{
    return false;
}

void SorceressOnBlackDragon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar