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
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Cogsmith::s_registered = false;

Unit *Cogsmith::Create(const ParameterList &parameters)
{
    auto unit = new Cogsmith();

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

std::string Cogsmith::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Cogsmith::EnumStringToInt(const std::string &enumString)
{
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
}

bool Cogsmith::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_grudgeRaker);
    model.addMissileWeapon(&m_pistols);
    model.addMeleeWeapon(&m_cogAxe);
    model.addMeleeWeapon(&m_gunButt);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Cogsmith::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_grudgeRaker);
    visitor(m_pistols);
    visitor(m_cogAxe);
    visitor(m_gunButt);
}

} //namespace CitiesOfSigmar