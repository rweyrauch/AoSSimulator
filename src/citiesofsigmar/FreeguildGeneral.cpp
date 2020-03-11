/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGeneral.h"

namespace CitiesOfSigmar
{

bool FreeguildGeneral::s_registered = false;

Unit *FreeguildGeneral::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildGeneral();

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

std::string FreeguildGeneral::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildGeneral::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildGeneral::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Freeguild General", *factoryMethod);
    }
}

FreeguildGeneral::FreeguildGeneral() :
    CitizenOfSigmar("Freeguild General", 5, WOUNDS, 7, 4, false),
    m_zweihander(Weapon::Type::Melee, "Zweihander", 1, 3, 3, 3, -2, RAND_D3)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, HERO, FREEGUILD_GENERAL};
    m_weapons = {&m_zweihander};

    s_globalBraveryMod.connect(this, &FreeguildGeneral::inspiringLeader, &m_connection);
}

FreeguildGeneral::~FreeguildGeneral()
{
    m_connection.disconnect();
}

bool FreeguildGeneral::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_zweihander);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds FreeguildGeneral::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Decapitating Swing
    auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    if (hitRoll == 6) damage.mortal++;
    return damage;
}

int FreeguildGeneral::inspiringLeader(const Unit *target)
{
    // Inspiring Leader
    if (target->hasKeyword(FREEGUILD) && (target->owningPlayer() == owningPlayer()) && (distanceTo(target) <= 18.0f))
    {
        return 1;
    }

    return 0;
}

} // namespace CitiesOfSigmar