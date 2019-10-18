/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/HelstormRocketBattery.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    HelstormRocketBattery::Create,
    HelstormRocketBattery::ValueToString,
    HelstormRocketBattery::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool HelstormRocketBattery::s_registered = false;


Unit *HelstormRocketBattery::Create(const ParameterList &parameters)
{
    auto unit = new HelstormRocketBattery();

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

std::string HelstormRocketBattery::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int HelstormRocketBattery::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void HelstormRocketBattery::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Helstorm Rocket Battery", factoryMethod);
    }
}

HelstormRocketBattery::HelstormRocketBattery() :
    CitizenOfSigmar("Helstorm Rocket Battery", 3, WOUNDS, 5, 4, false),
    m_rocketSalvo(Weapon::Type::Missile, "Helstorm Rocket Salvo", 36, 3, 5, 3, -2, RAND_D3),
    m_crewsTools(Weapon::Type::Melee, "Crew's Tools", 1, 3, 5, 5, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, HELSTORM_ROCKET_BATTERY};
}

bool HelstormRocketBattery::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_rocketSalvo);
    model.addMeleeWeapon(&m_crewsTools);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void HelstormRocketBattery::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_rocketSalvo);
    visitor(m_crewsTools);
}

} // namespace CitiesOfSigmar
