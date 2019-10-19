/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/ShadowWarriors.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    ShadowWarriors::Create,
    ShadowWarriors::ValueToString,
    ShadowWarriors::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", ShadowWarriors::MIN_UNIT_SIZE, ShadowWarriors::MIN_UNIT_SIZE,
            ShadowWarriors::MAX_UNIT_SIZE, ShadowWarriors::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool ShadowWarriors::s_registered = false;

Unit *ShadowWarriors::Create(const ParameterList &parameters)
{
    auto unit = new ShadowWarriors();

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

std::string ShadowWarriors::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int ShadowWarriors::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void ShadowWarriors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Shadow Warriors", factoryMethod);
    }
}

ShadowWarriors::ShadowWarriors() :
    CitizenOfSigmar("Shadow Warriors", 6, WOUNDS, 6, 5, false),
    m_bow(Weapon::Type::Missile, "Ranger Bow", 18, 1, 3, 4, -1, 1),
    m_blade(Weapon::Type::Melee, "Coldsteel Blade", 1, 2, 3, 4, 0, 1),
    m_bowWalker(Weapon::Type::Missile, "Ranger Bow", 18, 1, 2, 4, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SHADOWBLADES, SHADOW_WARRIORS};
}

bool ShadowWarriors::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Walker
    Model bossModel(BASESIZE, WOUNDS);
    bossModel.addMissileWeapon(&m_bowWalker);
    bossModel.addMeleeWeapon(&m_blade);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_bow);
        model.addMeleeWeapon(&m_blade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void ShadowWarriors::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_bow);
    visitor(m_blade);
    visitor(m_bowWalker);
}

} // namespace CitiesOfSigmar