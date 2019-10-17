/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildCrossbowmen.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildCrossbowmen::Create,
    FreeguildCrossbowmen::ValueToString,
    FreeguildCrossbowmen::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildCrossbowmen::s_registered = false;

Unit *FreeguildCrossbowmen::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildCrossbowmen();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool piper = GetBoolParam("Piper", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, piper);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FreeguildCrossbowmen::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildCrossbowmen::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildCrossbowmen::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild Crossbowmen", factoryMethod);
    }
}

FreeguildCrossbowmen::FreeguildCrossbowmen() :
    CitizenOfSigmar("Freeguild Crossbowmen", 5, WOUNDS, 5, 6, false),
    m_crossbow(Weapon::Type::Missile, "Freeguild Crossbow", 24, 1, 4, 3, 0, 1),
    m_dagger(Weapon::Type::Melee, "Dagger", 1, 1, 5, 5, 0, 1),
    m_crossbowMarksman(Weapon::Type::Missile, "Freeguild Crossbow", 24, 1, 3, 3, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_CROSSBOWMEN};
}

bool FreeguildCrossbowmen::configure(int numModels, bool standardBearer, bool piper)
{
    return false;
}

void FreeguildCrossbowmen::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_crossbow);
    visitor(m_dagger);
    visitor(m_crossbowMarksman);
}

} // namespace CitiesOfSigmar