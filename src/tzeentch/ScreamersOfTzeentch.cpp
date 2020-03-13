/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/ScreamersOfTzeentch.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{
static FactoryMethod factoryMethod = {
    ScreamersOfTzeentch::Create,
    TzeentchBase::ValueToString,
    TzeentchBase::EnumStringToInt,
    ScreamersOfTzeentch::ComputePoints,
    {
        {
            ParamType::Integer, "Models", ScreamersOfTzeentch::MIN_UNIT_SIZE, ScreamersOfTzeentch::MIN_UNIT_SIZE,
            ScreamersOfTzeentch::MAX_UNIT_SIZE, ScreamersOfTzeentch::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
    },
    CHAOS,
    { TZEENTCH }
};

bool ScreamersOfTzeentch::s_registered = false;

ScreamersOfTzeentch::ScreamersOfTzeentch() :
    TzeentchBase("Screamers of Tzeentch", 16, WOUNDS, 10, 5, true),
    m_bite(Weapon::Type::Melee, "Lamprey Bite", 1, 3, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, TZEENTCH, SCREAMERS};
    m_weapons = {&m_bite};
}

bool ScreamersOfTzeentch::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_bite);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *ScreamersOfTzeentch::Create(const ParameterList &parameters)
{
    auto unit = new ScreamersOfTzeentch();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto coven = (ChangeCoven)GetEnumParam("Change Coven", parameters, TzeentchBase::None);
    unit->setChangeCoven(coven);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ScreamersOfTzeentch::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Screamers of Tzeentch", factoryMethod);
    }
}

Wounds ScreamersOfTzeentch::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Sky-sharks
    if (target->hasKeyword(MONSTER)) {
        return { Dice::rollD3(), 0};
    }
    return TzeentchBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int ScreamersOfTzeentch::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Tzeentch