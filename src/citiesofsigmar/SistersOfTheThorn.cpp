/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/SistersOfTheThorn.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    SistersOfTheThorn::Create,
    SistersOfTheThorn::ValueToString,
    SistersOfTheThorn::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool SistersOfTheThorn::s_registered = false;

Unit *SistersOfTheThorn::Create(const ParameterList &parameters)
{
    auto unit = new SistersOfTheThorn();

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

std::string SistersOfTheThorn::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int SistersOfTheThorn::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void SistersOfTheThorn::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sisters of the Thorn", factoryMethod);
    }
}

SistersOfTheThorn::SistersOfTheThorn() :
    CitizenOfSigmar("Sisters of the Thorn", 12, WOUNDS, 7, 5, false),
    m_javelin(Weapon::Type::Missile, "Blackbriar Javelin", 9, 2, 4, 4, -1, 1),
    m_staff(Weapon::Type::Melee, "Deepwood Coven Staff", 2, 1, 4, 4, 0, 1),
    m_antlersAndHooves(Weapon::Type::Melee, "Antlers and Horns", 1, 2, 4, 4, 0, 1),
    m_staffMaiden(Weapon::Type::Melee, "Deepwood Coven Staff", 2, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, WIZARD, SISTERS_OF_THE_THORN};
}

bool SistersOfTheThorn::configure(int numModels, bool standardBearer, bool hornblower)
{
    return false;
}

void SistersOfTheThorn::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_javelin);
    visitor(m_staff);
    visitor(m_antlersAndHooves);
    visitor(m_staffMaiden);
}

} // namespace CitiesOfSigmar