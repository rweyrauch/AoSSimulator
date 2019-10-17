/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGuard.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildGuard::Create,
    FreeguildGuard::ValueToString,
    FreeguildGuard::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildGuard::s_registered = false;

Unit *FreeguildGuard::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildGuard();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool drummer = GetBoolParam("Drummer", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FreeguildGuard::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildGuard::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild Guard", factoryMethod);
    }
}

FreeguildGuard::FreeguildGuard() :
    CitizenOfSigmar("Freeguild Guard", 5, WOUNDS, 5, 5, false),
    m_halberd(Weapon::Type::Melee, "Freeguild Halberd", 1, 1, 4, 3, -1, 1),
    m_spear(Weapon::Type::Melee, "Freeguild Spear", 2, 1, 4, 4, 0, 1),
    m_sword(Weapon::Type::Melee, "Freeguild Sword", 1, 1, 4, 4, 0, 1),
    m_halberdSergeant(Weapon::Type::Melee, "Freeguild Halberd", 1, 2, 4, 3, -1, 1),
    m_spearSergeant(Weapon::Type::Melee, "Freeguild Spear", 2, 2, 4, 4, 0, 1),
    m_swordSergeant(Weapon::Type::Melee, "Freeguild Sword", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_GUARD};
}

bool FreeguildGuard::configure(int numModels, bool standardBearer, bool drummer)
{
    return false;
}

void FreeguildGuard::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_halberd);
    visitor(m_spear);
    visitor(m_sword);
    visitor(m_halberdSergeant);
    visitor(m_spearSergeant);
    visitor(m_swordSergeant);
}

} // namespace CitiesOfSigmar