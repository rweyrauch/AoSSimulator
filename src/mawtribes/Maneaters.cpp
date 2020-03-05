/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Maneaters.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    Maneaters::Create,
    Maneaters::ValueToString,
    Maneaters::EnumStringToInt,
    Maneaters::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Maneaters::MIN_UNIT_SIZE, Maneaters::MIN_UNIT_SIZE,
            Maneaters::MAX_UNIT_SIZE, Maneaters::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Ability", Maneaters::Brawlers, Maneaters::Brawlers, Maneaters::Stubborn, 1},
    },
    DESTRUCTION,
    { OGOR_MAWTRIBES }
};

bool Maneaters::s_registered = false;

Unit *Maneaters::Create(const ParameterList &parameters)
{
    auto unit = new Maneaters();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto ability = (Ability)GetEnumParam("Ability", parameters, Brawlers);

    bool ok = unit->configure(numModels, ability);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Maneaters::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Ability")
    {
        if (parameter.intValue == Brawlers) return "Brawlers";
        else if (parameter.intValue == CrackShots) return "Crack Shots";
        else if (parameter.intValue == Striders) return "Striders";
        else if (parameter.intValue == Stubborn) return "Stubborn";
    }
    return MawtribesBase::ValueToString(parameter);
}

int Maneaters::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Brawlers") return Brawlers;
    else if (enumString == "Crack Shots") return CrackShots;
    else if (enumString == "Striders") return Striders;
    else if (enumString == "Stubborn") return Stubborn;

    return MawtribesBase::EnumStringToInt(enumString);
}

void Maneaters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Maneaters", factoryMethod);
    }
}

Maneaters::Maneaters() :
    MawtribesBase("Maneaters", 6, WOUNDS, 7, 5, false),
    m_pistolsOrStars(Weapon::Type::Missile, "Pistols or Throwing Stars", 12, 1, 3, 3, -1, RAND_D3),
    m_bashers(Weapon::Type::Melee, "Slicers and Bashers", 1, 4, 3, 3, -1, 2),
    m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, MANEATERS};
    m_weapons = {&m_pistolsOrStars, &m_bite, &m_bashers};
}

bool Maneaters::configure(int numModels, Ability ability)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_ability = ability;

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_pistolsOrStars);
        model->addMeleeWeapon(&m_bashers);
        model->addMeleeWeapon(&m_bite);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int Maneaters::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace OgorMawtribes
