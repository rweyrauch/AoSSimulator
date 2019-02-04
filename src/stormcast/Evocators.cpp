/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/Evocators.h>
#include <iostream>

namespace StormcastEternals
{

static FactoryMethod factoryMethod = {
    Evocators::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = 5}, Evocators::MIN_UNIT_SIZE, Evocators::MAX_UNIT_SIZE, Evocators::MIN_UNIT_SIZE},
        {ParamType::Boolean, "primeGrandstave", {.m_boolValue = false}, false, false},
        {ParamType::Integer, "numGrandstaves", {.m_intValue = 2}, 0, Evocators::MAX_UNIT_SIZE},
    }
};

bool Evocators::s_registered = false;

Evocators::Evocators() :
    StormcastEternal("Evocators", 5, WOUNDS, 8, 4, false),
    m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1),
    m_tempestBladeAndStavePrime(Weapon::Type::Melee, "Tempest Blade and Stormstave (Prime)", 1, 5, 3, 3, -1, 1),
    m_grandStave(Weapon::Type::Melee, "Grandstave", 2, 3, 3, 3, 0, 2),
    m_grandStavePrime(Weapon::Type::Melee, "Grandstave (Prime)", 2, 4, 3, 3, 0, 2)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, RETRIBUTORS };
}

bool Evocators::configure(int numModels, int numGrandstaves, bool primeGrandstave)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    if (primeGrandstave && numGrandstaves <= 0)
    {
        // Must have one stave.
        return false;
    }

    const int maxGrandstaves = numModels;
    if (numGrandstaves > maxGrandstaves)
    {
        // Invalid weapon configuration.
        return false;
    }

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (primeGrandstave)
    {
        primeModel.addMeleeWeapon(&m_grandStavePrime);
        numGrandstaves--;
    }
    else
        primeModel.addMeleeWeapon(&m_tempestBladeAndStavePrime);
    addModel(primeModel);

    for (auto i = 0; i < numGrandstaves; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_grandStave);
        addModel(model);
    }

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_tempestBladeAndStave);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

Rerolls Evocators::toSaveRerolls(const Weapon* weapon) const
{
    // Celestial Lightning Arc
    if (weapon->isMissile())
        return RerollOnes;

    return StormcastEternal::toSaveRerolls(weapon);
}

int Evocators::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits, const WoundingHits& wounds)
{
    int mortalWounds = Unit::generateMortalWounds(weapon, unit, hits, wounds);

    // Celestial Lightning Arc
    Dice dice;
    int roll = dice.rollD6();
    if (roll >= 4)
        mortalWounds++;
    roll = dice.rollD6();
    if (roll >= 4)
        mortalWounds++;

    return mortalWounds;
}

Unit *Evocators::Create(const ParameterList &parameters)
{
    auto *evos = new Evocators();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool primeGrandstave = GetBoolParam("primeGrandstave", parameters, false);
    int numGrandstaves = GetIntParam("numGrandstaves", parameters, 0);

    bool ok = evos->configure(numModels, numGrandstaves, primeGrandstave);
    if (!ok)
    {
        delete evos;
        evos = nullptr;
    }
    return evos;
}

void Evocators::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Evocators", factoryMethod);
    }
}

void Evocators::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_tempestBladeAndStave);
    visitor(&m_tempestBladeAndStavePrime);
    visitor(&m_grandStave);
    visitor(&m_grandStavePrime);
}

} // namespace StormcastEternals
