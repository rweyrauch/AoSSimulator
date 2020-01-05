/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Assassin.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Assassin::Create,
    Assassin::ValueToString,
    Assassin::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Assassin::s_registered = false;

Unit *Assassin::Create(const ParameterList &parameters)
{
    auto unit = new Assassin();

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

std::string Assassin::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Assassin::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Assassin::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Assassin", factoryMethod);
    }
}

Assassin::Assassin() :
    CitizenOfSigmar("Assassin", 6, WOUNDS, 7, 5, false),
    m_blades(Weapon::Type::Melee, "Poison-coated Blades", 1, 6, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SHADOWBLADES, HERO, ASSASSIN};
    m_weapons = {&m_blades};
}

bool Assassin::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_blades);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds Assassin::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Deathshead Poison
    if ((woundRoll == 6) && (weapon->name() == m_blades.name()))
    {
        Dice dice;
        return { 0,  dice.rollD3() };
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace CitiesOfSigmar