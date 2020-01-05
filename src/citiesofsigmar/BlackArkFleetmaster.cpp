/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/BlackArkFleetmaster.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    BlackArkFleetmaster::Create,
    BlackArkFleetmaster::ValueToString,
    BlackArkFleetmaster::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool BlackArkFleetmaster::s_registered = false;

Unit *BlackArkFleetmaster::Create(const ParameterList &parameters)
{
    auto unit = new BlackArkFleetmaster();

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

std::string BlackArkFleetmaster::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int BlackArkFleetmaster::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void BlackArkFleetmaster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Black Ark Fleetmaster", factoryMethod);
    }
}

BlackArkFleetmaster::BlackArkFleetmaster() :
    CitizenOfSigmar("Black Ark Fleetmaster", 6, WOUNDS, 7, 4, false),
    m_cutlass(Weapon::Type::Melee, "Black Ark Cutlass", 1, 3, 3, 4, 0, 1),
    m_murderHook(Weapon::Type::Melee, "Murder Hook", 1, 2, 4, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, HERO, BLACK_ARK_FLEETMASTER};
    m_weapons = {&m_cutlass, &m_murderHook};
}

bool BlackArkFleetmaster::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_cutlass);
    model->addMeleeWeapon(&m_murderHook);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int BlackArkFleetmaster::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Murderous Swashbuckler
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_cutlass.name()))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

}//namespace CitiesOfSigmar