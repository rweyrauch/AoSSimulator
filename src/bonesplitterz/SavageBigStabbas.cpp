/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageBigStabbas.h"

namespace Bonesplitterz
{

bool SavageBigStabbas::s_registered = false;

Unit *SavageBigStabbas::Create(const ParameterList &parameters)
{
    auto unit = new SavageBigStabbas();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
    unit->setWarclan(warclan);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SavageBigStabbas::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Bonesplitterz::ValueToString,
            Bonesplitterz::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz, Bonesplitterz::Icebone, 1},
            },
            DESTRUCTION,
            { BONESPLITTERZ }
        };

        s_registered = UnitFactory::Register("Savage Big Stabbas", *factoryMethod);
    }
}

SavageBigStabbas::SavageBigStabbas() :
    Bonesplitterz("Savage Big Stabbas", 5, WOUNDS, 6, 6, false),
    m_gorkToof(Weapon::Type::Melee, "Gorktoof", 3, 3, 3, 3, -2, RAND_D3)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_BIG_STABBAS};
    m_weapons = { &m_gorkToof };
    // Savagely Enthusiastic
    m_runAndCharge = true;
}

void SavageBigStabbas::onModelSlain()
{
    // Da Final Fling
    // TODO: check if slain by a melee weapon
}

bool SavageBigStabbas::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_gorkToof);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Wounds SavageBigStabbas::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // The Bigger They Are
    if (target->hasKeyword(MONSTER))
    {
        return {Dice::rollSpecial(RAND_D6), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int SavageBigStabbas::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Bonesplitterz
