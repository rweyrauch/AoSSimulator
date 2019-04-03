/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Decimators.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Decimators::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "Models", Decimators::MIN_UNIT_SIZE, Decimators::MIN_UNIT_SIZE, Decimators::MAX_UNIT_SIZE, Decimators::MIN_UNIT_SIZE},
        {ParamType::Integer, "Starsoul Maces", 2, 0, (Decimators::MAX_UNIT_SIZE / 5) * 2, 1}
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Decimators::s_registered = false;

Decimators::Decimators() :
    StormcastEternal("Decimators", 4, WOUNDS, 7, 4, false),
    m_thunderaxe(Weapon::Type::Melee, "Thunderaxe", 2, 0, 3, 3, -1, 1),
    m_thunderaxePrime(Weapon::Type::Melee, "Thunderaxe (Prime)", 2, 0, 3, 3, -1, 1),
    m_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 1, 0, 0, 0, 0)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, DECIMATORS};
}

bool Decimators::configure(int numModels, int numStarsoulMaces)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxStarsoulMaces = (numModels / 5) * 2;
    if (numStarsoulMaces > maxStarsoulMaces)
    {
        // Invalid weapon configuration.
        return false;
    }

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    primeModel.addMeleeWeapon(&m_thunderaxePrime);
    addModel(primeModel);

    for (auto i = 0; i < numStarsoulMaces; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_starsoulMace);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_thunderaxe);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Wounds Decimators::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Starsoul Mace
    if (weapon->name() == m_starsoulMace.name())
    {
        int mortalWounds = 0;
        Dice dice;
        int roll = dice.rollD6();
        if (roll >= 6)
        {
            mortalWounds = dice.rollD3() + 1;
        }
        else if (roll >= 2)
        {
            mortalWounds = dice.rollD3();
        }
        return {0, mortalWounds};
    }
    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Unit *Decimators::Create(const ParameterList &parameters)
{
    auto unit = new Decimators();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numStarsoulMaces = GetIntParam("Starsoul Maces", parameters, 0);

    bool ok = unit->configure(numModels, numStarsoulMaces);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Decimators::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Decimators", factoryMethod);
    }
}

void Decimators::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_thunderaxe);
    visitor(&m_thunderaxePrime);
    visitor(&m_starsoulMace);
}

int Decimators::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    if (weapon->name() == m_thunderaxe.name())
    {
        // count the number of models in the unit w/in 2 in of this model
        return target->numModelsWithin(attackingModel, weapon->range());
    }
    return StormcastEternal::extraAttacks(nullptr, weapon, target);
}


} // namespace StormcastEternals
