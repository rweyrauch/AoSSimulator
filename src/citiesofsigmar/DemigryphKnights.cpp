/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DemigryphKnights.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    DemigryphKnights::Create,
    DemigryphKnights::ValueToString,
    DemigryphKnights::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool DemigryphKnights::s_registered = false;

Unit *DemigryphKnights::Create(const ParameterList &parameters)
{
    auto unit = new DemigryphKnights();

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

std::string DemigryphKnights::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int DemigryphKnights::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void DemigryphKnights::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Demigryph Knights", factoryMethod);
    }
}

DemigryphKnights::DemigryphKnights() :
    CitizenOfSigmar("Demigryph Knights", 10, WOUNDS, 6, 3, false),
    m_halberd(Weapon::Type::Melee, "Demigryph Knight's Halberd", 2, 3, 3, 3, -1, 1),
    m_lance(Weapon::Type::Melee, "Demigryph Knight's Lance", 2, 3, 3, 4, 0, 1),
    m_halberdPreceptor(Weapon::Type::Melee, "Demigryph Knight's Halberd", 2, 4, 3, 3, -1, 1),
    m_lancePreceptor(Weapon::Type::Melee, "Demigryph Knight's Lance", 2, 4, 3, 4, 0, 1),
    m_beakAndTalons(Weapon::Type::Melee, "Beak and Talons", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, DEMIGRYPH_KNIGHTS};
}

bool DemigryphKnights::configure(int numModels, bool standardBearer, bool hornblower)
{
    return false;
}

void DemigryphKnights::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar
