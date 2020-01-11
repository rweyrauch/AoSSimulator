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
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", ScreamersOfTzeentch::MIN_UNIT_SIZE, ScreamersOfTzeentch::MIN_UNIT_SIZE,
            ScreamersOfTzeentch::MAX_UNIT_SIZE, ScreamersOfTzeentch::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    TZEENTCH
};

bool ScreamersOfTzeentch::s_registered = false;

ScreamersOfTzeentch::ScreamersOfTzeentch() :
    Unit("Screamers of Tzeentch", 16, WOUNDS, 10, 5, true),
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

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *ScreamersOfTzeentch::Create(const ParameterList &parameters)
{
    auto unit = new ScreamersOfTzeentch();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
        Dice dice;
        return { dice.rollD3(), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace Tzeentch