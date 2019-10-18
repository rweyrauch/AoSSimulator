/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/HelblasterVolleyGun.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    HelblasterVolleyGun::Create,
    HelblasterVolleyGun::ValueToString,
    HelblasterVolleyGun::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool HelblasterVolleyGun::s_registered = false;

Unit *HelblasterVolleyGun::Create(const ParameterList &parameters)
{
    auto unit = new HelblasterVolleyGun();

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

std::string HelblasterVolleyGun::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int HelblasterVolleyGun::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void HelblasterVolleyGun::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Helblaster Volley Gun", factoryMethod);
    }
}

HelblasterVolleyGun::HelblasterVolleyGun() :
    CitizenOfSigmar("Helblaster Volley Gun", 3, WOUNDS, 5, 4, false),
    m_volley1(Weapon::Type::Missile, "Volley of Shots (1 Deck)", 24, RAND_D6, 4, 3, -1, 1),
    m_volley2(Weapon::Type::Missile, "Volley of Shots (2 Decks)", 24, RAND_2D6, 4, 3, -1, 1),
    m_volley3(Weapon::Type::Missile, "Volley of Shots (3 Decks)", 24, RAND_3D6, 4, 3, -1, 1),
    m_crewsTools(Weapon::Type::Melee, "Crew's Tools", 1, 3, 5, 5, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, HELBLASTER_VOLLEY_GUN};
}

bool HelblasterVolleyGun::configure()
{
    Model model(BASESIZE, WOUNDS);

    // TODO: allow selection of 1, 2 or 3 decks
    model.addMissileWeapon(&m_volley1);
    model.addMeleeWeapon(&m_crewsTools);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void HelblasterVolleyGun::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_volley1);
    visitor(m_volley2);
    visitor(m_volley3);
    visitor(m_crewsTools);
}

} // namespace CitiesOfSigmar