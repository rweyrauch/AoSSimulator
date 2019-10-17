/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildHandgunners.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildHandgunners::Create,
    FreeguildHandgunners::ValueToString,
    FreeguildHandgunners::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildHandgunners::s_registered = false;

Unit *FreeguildHandgunners::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildHandgunners();

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

std::string FreeguildHandgunners::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildHandgunners::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildHandgunners::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild Handgunners", factoryMethod);
    }
}

FreeguildHandgunners::FreeguildHandgunners() :
    CitizenOfSigmar("Freeguild Handgunners", 5, WOUNDS, 5, 6, false),
    m_longRifle(Weapon::Type::Missile, "Long Rifle", 30, 1, 4, 3, -1, 2),
    m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
    m_freeguildHandgun(Weapon::Type::Missile, "Freeguild Handgun", 16, 1, 4, 3, -1, 1),
    m_dagger(Weapon::Type::Melee, "Dagger", 1, 1, 5, 5, 0, 1),
    m_handgunMarksman(Weapon::Type::Missile, "Freeguild Handgun", 16, 1, 2, 3, -1, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_HANDGUNNERS};
}

bool FreeguildHandgunners::configure(int numModels, bool standardBearer, bool hornblower)
{
    return false;
}

void FreeguildHandgunners::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar