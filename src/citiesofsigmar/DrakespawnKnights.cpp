/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DrakespawnKnights.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    DrakespawnKnights::Create,
    DrakespawnKnights::ValueToString,
    DrakespawnKnights::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool DrakespawnKnights::s_registered = false;

Unit *DrakespawnKnights::Create(const ParameterList &parameters)
{
    auto unit = new DrakespawnKnights();

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

std::string DrakespawnKnights::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int DrakespawnKnights::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void DrakespawnKnights::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Drakespawn Knights", factoryMethod);
    }
}

DrakespawnKnights::DrakespawnKnights() :
    CitizenOfSigmar("Drakespawn Knights", 10, WOUNDS, 7, 3, false),
    m_lance(Weapon::Type::Melee, "Barbed Lance", 2, 1, 3, 4, -1, 1),
    m_lanceDreadKnight(Weapon::Type::Melee, "Barbed Lance", 2, 2, 3, 4, -1, 1),
    m_jaws(Weapon::Type::Melee, "Ferocious Jaws", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, DRAKESPAWN_KNIGHTS};
}

bool DrakespawnKnights::configure(int numModels, bool standardBearer, bool hornblower)
{
    return false;
}

void DrakespawnKnights::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar