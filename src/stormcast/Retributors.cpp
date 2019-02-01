/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Retributors.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Retributors::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = 5}, Retributors::MIN_UNIT_SIZE, Retributors::MAX_UNIT_SIZE, Retributors::MIN_UNIT_SIZE},
        {ParamType::Integer, "numStarsoulMaces", {.m_intValue = 2}, 0, (Retributors::MAX_UNIT_SIZE / 5) * 2}
    }
};

bool Retributors::s_registered = false;

Weapon Retributors::s_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 2, 3, 3, -1, 2);
Weapon Retributors::s_lightningHammerPrime(Weapon::Type::Melee, "Lightning Hammer (Prime)", 1, 3, 3, 3, -1, 2);
Weapon Retributors::s_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 1, 0, 0, 0, 0);

Retributors::Retributors() :
    StormcastEternal("Retributors", 4, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, RETRIBUTORS };
}

bool Retributors::configure(int numModels, int numStarsoulMaces)
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
    primeModel.addMeleeWeapon(&s_lightningHammerPrime);
    addModel(primeModel);

    for (auto i = 0; i < numStarsoulMaces; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_starsoulMace);
        addModel(model);
    }

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_lightningHammer);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

int Retributors::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits)
{
    // Blast to Ashes
    if (weapon->name() == s_lightningHammer.name())
    {
        int mortalWounds = hits.rolls.numUnmodified6s() * 2;
        /*
        if (mortalWounds)
            std::cout << "Blast to Ashes did " << mortalWounds << " mortal wounds." << std::endl;
        */
        return mortalWounds;
    }

    // Starsoul Mace
    if (weapon->name() == s_starsoulMace.name())
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
/*
        if (mortalWounds)
            std::cout << "Starsoul Mace did " << mortalWounds << " mortal wounds." << std::endl;
*/
        return mortalWounds;
    }
    return StormcastEternal::generateMortalWounds(weapon, unit, hits);
}

Unit *Retributors::Create(const ParameterList &parameters)
{
    auto unit = new Retributors();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    int numStarsoulMaces = GetIntParam("numStarsoulMaces", parameters, 0);

    bool ok = unit->configure(numModels, numStarsoulMaces);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Retributors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Retributors", factoryMethod);
    }
}

void Retributors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_lightningHammer);
    visitor(&s_lightningHammerPrime);
    visitor(&s_starsoulMace);
}

} // namespace StormcastEternals
