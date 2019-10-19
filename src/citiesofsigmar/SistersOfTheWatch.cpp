/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/SistersOfTheWatch.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    SistersOfTheWatch::Create,
    SistersOfTheWatch::ValueToString,
    SistersOfTheWatch::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", SistersOfTheWatch::MIN_UNIT_SIZE, SistersOfTheWatch::MIN_UNIT_SIZE,
            SistersOfTheWatch::MAX_UNIT_SIZE, SistersOfTheWatch::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool SistersOfTheWatch::s_registered = false;

Unit *SistersOfTheWatch::Create(const ParameterList &parameters)
{
    auto unit = new SistersOfTheWatch();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string SistersOfTheWatch::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int SistersOfTheWatch::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void SistersOfTheWatch::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sisters of the Watch", factoryMethod);
    }
}

SistersOfTheWatch::SistersOfTheWatch() :
    CitizenOfSigmar("Sisters of the Watch", 6, WOUNDS, 7, 5, false),
    m_bow(Weapon::Type::Missile, "Watch Bow", 18, 1, 3, 3, 0, 1),
    m_sword(Weapon::Type::Melee, "Ithilmar Sword", 1, 1, 4, 4, 0, 1),
    m_bowHighSister(Weapon::Type::Missile, "Watch Bow", 18, 2, 3, 3, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, SISTERS_OF_THE_WATCH};
}

bool SistersOfTheWatch::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the High Sister
    Model bossModel(BASESIZE, WOUNDS);
    bossModel.addMissileWeapon(&m_bowHighSister);
    bossModel.addMeleeWeapon(&m_sword);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_bow);
        model.addMeleeWeapon(&m_sword);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SistersOfTheWatch::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_bow);
    visitor(m_sword);
    visitor(m_bowHighSister);
}

} // namespace CitiesOfSigmar