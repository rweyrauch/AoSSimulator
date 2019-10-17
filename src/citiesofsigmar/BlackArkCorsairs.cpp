/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/BlackArkCorsairs.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    BlackArkCorsairs::Create,
    BlackArkCorsairs::ValueToString,
    BlackArkCorsairs::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool BlackArkCorsairs::s_registered = false;

Unit *BlackArkCorsairs::Create(const ParameterList &parameters)
{
    auto unit = new BlackArkCorsairs();

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

std::string BlackArkCorsairs::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int BlackArkCorsairs::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void BlackArkCorsairs::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Black Ark Corsairs", factoryMethod);
    }
}

BlackArkCorsairs::BlackArkCorsairs() :
    CitizenOfSigmar("Black Ark Corsairs", 6, WOUNDS, 6, 5, false),
    m_handbow(Weapon::Type::Missile, "Repeater Handbow", 9, 2, 5, 4, 0, 1),
    m_cutlass(Weapon::Type::Melee, "Wicked Cutlass", 1, 1, 4, 4, 0, 1),
    m_blade(Weapon::Type::Melee, "Vicious Blade", 1, 1, 4, 5, 0, 1),
    m_handbowReaver(Weapon::Type::Missile, "Repeater Handbow", 9, 2, 4, 4, 0, 1),
    m_cutlassReaver(Weapon::Type::Melee, "Wicked Cutlass", 1, 1, 3, 4, 0, 1),
    m_bladeReaver(Weapon::Type::Melee, "Vicious Blade", 1, 1, 3, 5, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, BLACK_ARK_CORSAIRS};
}

bool BlackArkCorsairs::configure(int numModels, bool standardBearer, bool hornblower)
{
    return false;
}

void BlackArkCorsairs::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

}//namespace CitiesOfSigmar