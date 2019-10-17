/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FlamespyrePhoenix.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FlamespyrePhoenix::Create,
    FlamespyrePhoenix::ValueToString,
    FlamespyrePhoenix::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FlamespyrePhoenix::s_registered = false;

Unit *FlamespyrePhoenix::Create(const ParameterList &parameters)
{
    auto unit = new FlamespyrePhoenix();

    auto anointed = GetBoolParam("Anointed", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(anointed);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FlamespyrePhoenix::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FlamespyrePhoenix::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FlamespyrePhoenix::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Flamespyre Phoenix", factoryMethod);
    }
}

FlamespyrePhoenix::FlamespyrePhoenix() :
    CitizenOfSigmar("Flamespyre Phoenix", 16, WOUNDS, 8, 4, true),
    m_talons(Weapon::Type::Melee, "Flaming Talons", 2, 6, 4, 3, -1, 2),
    m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, MONSTER, FLAMESPYRE_PHOENIX};
}

bool FlamespyrePhoenix::configure(bool anoited)
{
    if (anoited)
    {
        addKeyword(HERO);
    }

    return false;
}

void FlamespyrePhoenix::visitWeapons(std::function<void(const Weapon *)> &visitor)
{

}

} // namespace CitiesOfSigmar