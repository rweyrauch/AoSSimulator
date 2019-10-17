/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Dreadspears.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Dreadspears::Create,
    Dreadspears::ValueToString,
    Dreadspears::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Dreadspears::s_registered = false;

Unit *Dreadspears::Create(const ParameterList &parameters)
{
    auto unit = new Dreadspears();

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

std::string Dreadspears::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Dreadspears::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Dreadspears::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dreadspears", factoryMethod);
    }
}

Dreadspears::Dreadspears() :
    CitizenOfSigmar("Dreadspears", 6, WOUNDS, 6, 4, false),
    m_spear(Weapon::Type::Melee, "Darkling Spear", 2, 1, 4, 4, 0, 1),
    m_spearLordling(Weapon::Type::Melee, "Darkling Spear", 2, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, DREADSPEARS};
}

bool Dreadspears::configure(int numModels, bool standardBearer, bool hornblower)
{
    return false;
}

void Dreadspears::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_spear);
    visitor(m_spearLordling);
}

} // namespace CitiesOfSigmar