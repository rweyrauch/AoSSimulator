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
static FactoryMethod factoryMethod = {
    SavageBigStabbas::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "Models", SavageBigStabbas::MIN_UNIT_SIZE, SavageBigStabbas::MIN_UNIT_SIZE, SavageBigStabbas::MAX_UNIT_SIZE, SavageBigStabbas::MIN_UNIT_SIZE},
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageBigStabbas::s_registered = false;

Unit *SavageBigStabbas::Create(const ParameterList &parameters)
{
    auto unit = new SavageBigStabbas();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
        s_registered = UnitFactory::Register("Savage Big Stabbas", factoryMethod);
    }
}

SavageBigStabbas::SavageBigStabbas() :
    Unit("Savage Big Stabbas", 5, WOUNDS, 6, 6, false),
    m_gorkToof(Weapon::Type::Melee, "Gorktoof", 3, 3, 3, 3, -2, RAND_D3)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_BIG_STABBAS};

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
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_gorkToof);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SavageBigStabbas::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_gorkToof);
}

Wounds SavageBigStabbas::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // The Bigger They Are
    if (target->hasKeyword(MONSTER))
    {
        Dice dice;
        return {dice.rollSpecial(RAND_D6), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Bonesplitterz
