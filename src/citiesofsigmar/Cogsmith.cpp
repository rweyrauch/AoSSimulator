/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Cogsmith.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Cogsmith::Create,
    Cogsmith::ValueToString,
    Cogsmith::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapon", Cogsmith::GrudgeRaker, Cogsmith::GrudgeRaker,
            Cogsmith::CogAxe, 1
        },
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    { CITIES_OF_SIGMAR }
};

bool Cogsmith::s_registered = false;

Unit *Cogsmith::Create(const ParameterList &parameters)
{
    auto unit = new Cogsmith();

    auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, GrudgeRaker);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(weapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Cogsmith::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == GrudgeRaker)
        {
            return "Grudge-raker";
        }
        else if (parameter.intValue == CogAxe)
        {
            return "Cog Axe";
        }
    }

    return CitizenOfSigmar::ValueToString(parameter);
}

int Cogsmith::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Grudge-raker")
    {
        return GrudgeRaker;
    }
    else if (enumString == "Cog Axe")
    {
        return CogAxe;
    }
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Cogsmith::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Cogsmith", factoryMethod);
    }
}

Cogsmith::Cogsmith() :
    CitizenOfSigmar("Cogsmith", 4, WOUNDS, 7, 5, false),
    m_grudgeRaker(Weapon::Type::Missile, "Grudge-raker", 16, RAND_D3, 4, 3, -1, 1),
    m_pistols(Weapon::Type::Missile, "Duardin Pistols", 8, 2, 4, 3, -1, 1),
    m_cogAxe(Weapon::Type::Melee, "Cog Axe", 1, 4, 3, 4, 0, 1),
    m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, HERO, ENGINEER, COGSMITH};
    m_weapons = {&m_grudgeRaker, &m_pistols, &m_cogAxe, &m_gunButt};
}

bool Cogsmith::configure(WeaponOption weapon)
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_pistols);
    if (weapon == GrudgeRaker)
    {
        model->addMissileWeapon(&m_grudgeRaker);
    }

    model->addMeleeWeapon(&m_gunButt);
    if (weapon == CogAxe)
    {
        model->addMeleeWeapon(&m_cogAxe);
    }
    addModel(model);

    m_weaponOption = weapon;
    m_points = POINTS_PER_UNIT;

    return true;
}

int Cogsmith::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Free Arm
    if ((m_weaponOption == GrudgeRaker) && (weapon->name() == m_grudgeRaker.name())) mod++;
    else if ((m_weaponOption == CogAxe) && (weapon->name() == m_cogAxe.name())) mod++;

    return mod;
}

} //namespace CitiesOfSigmar