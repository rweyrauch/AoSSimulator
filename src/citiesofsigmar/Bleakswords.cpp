/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Bleakswords.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Bleakswords::Create,
    Bleakswords::ValueToString,
    Bleakswords::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Bleakswords::s_registered = false;

Unit *Bleakswords::Create(const ParameterList &parameters)
{
    auto unit = new Bleakswords();

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

std::string Bleakswords::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Bleakswords::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Bleakswords::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bleakswords", factoryMethod);
    }
}

Bleakswords::Bleakswords() :
    CitizenOfSigmar("Bleakswords", 6, WOUNDS, 6, 4, false),
    m_sword(Weapon::Type::Melee, "Darkling Sword", 1, 1, 4, 4, 0, 1),
    m_swordLordling(Weapon::Type::Melee, "Darkling Sword", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, BLEAKSWORDS};
}

bool Bleakswords::configure(int numModels, bool standardBearer, bool hornblower)
{
    return false;
}

void Bleakswords::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} //namespace CitiesOfSigmar