/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/WildRiders.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    WildRiders::Create,
    WildRiders::ValueToString,
    WildRiders::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool WildRiders::s_registered = false;

Unit *WildRiders::Create(const ParameterList &parameters)
{
    auto unit = new WildRiders();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool hornblower = GetBoolParam("Hornblower", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string WildRiders::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int WildRiders::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void WildRiders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Wild Riders", factoryMethod);
    }
}

WildRiders::WildRiders() :
    CitizenOfSigmar("Wild Riders", 12, WOUNDS, 8, 5, false),
    m_spear(Weapon::Type::Melee, "Hunting Spear", 2, 2, 3, 4, -1, 1),
    m_hooves(Weapon::Type::Melee, "Antlers and Hooves", 1, 2, 4, 4, 0, 1),
    m_spearHunter(Weapon::Type::Melee, "Hunting Spear", 2, 3, 3, 4, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, WILD_RIDERS};
}

bool WildRiders::configure(int numModels, bool standardBearer, bool hornblower)
{
    return false;
}

void WildRiders::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_spear);
    visitor(m_hooves);
    visitor(m_spearHunter);
}

} // namespace CitiesOfSigmar