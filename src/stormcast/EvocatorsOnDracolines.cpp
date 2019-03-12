/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/EvocatorsOnDracolines.h>
#include <iostream>

namespace StormcastEternals
{

static FactoryMethod factoryMethod = {
    EvocatorsOnCelestialDracolines::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "Models", {.m_intValue = EvocatorsOnCelestialDracolines::MIN_UNIT_SIZE}, EvocatorsOnCelestialDracolines::MIN_UNIT_SIZE,
         EvocatorsOnCelestialDracolines::MAX_UNIT_SIZE, EvocatorsOnCelestialDracolines::MIN_UNIT_SIZE},
        {ParamType::Boolean, "Prime Grandstave", {.m_boolValue = false}, false, false},
        {ParamType::Integer, "Grandstaves", {.m_intValue = 2}, 0, EvocatorsOnCelestialDracolines::MAX_UNIT_SIZE/EvocatorsOnCelestialDracolines::MIN_UNIT_SIZE*2, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool EvocatorsOnCelestialDracolines::s_registered = false;

EvocatorsOnCelestialDracolines::EvocatorsOnCelestialDracolines() :
    StormcastEternal("Evocators on Celestial Dracolines", 12, WOUNDS, 8, 4, false),
    m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1),
    m_tempestBladeAndStavePrime(Weapon::Type::Melee, "Tempest Blade and Stormstave (Prime)", 1, 5, 3, 3, -1, 1),
    m_grandStave(Weapon::Type::Melee, "Grandstave", 2, 3, 3, 3, 0, 2),
    m_grandStavePrime(Weapon::Type::Melee, "Grandstave (Prime)", 2, 4, 3, 3, 0, 2),
    m_monstrousClaws(Weapon::Type::Melee, "Monstrous Claws",1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOLINE, STORMCAST_ETERNAL, SACROSANCT, CORPUSCANT, WIZARD, EVOCATORS};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool EvocatorsOnCelestialDracolines::configure(int numModels, int numGrandstaves, bool primeGrandstave)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    const int maxGrandstaves = numModels / MIN_UNIT_SIZE * 2;
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
    {
        primeModel.addMeleeWeapon(&m_tempestBladeAndStavePrime);
    }
    primeModel.addMeleeWeapon(&m_monstrousClaws);
    addModel(primeModel);

    for (auto i = 0; i < numGrandstaves; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_grandStave);
        model.addMeleeWeapon(&m_monstrousClaws);
        addModel(model);
    }

    auto currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_tempestBladeAndStave);
        model.addMeleeWeapon(&m_monstrousClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Rerolls EvocatorsOnCelestialDracolines::toSaveRerolls(const Weapon *weapon) const
{
    // Celestial Lightning Arc
    if (weapon->isMissile())
    {
        return RerollOnes;
    }

    return StormcastEternal::toSaveRerolls(weapon);
}

int EvocatorsOnCelestialDracolines::generateMortalWounds(const Unit *unit)
{
    auto mortalWounds = StormcastEternal::generateMortalWounds(unit);

    // Celestial Lightning Arc
    Dice dice;
    int roll = dice.rollD6();
    if (roll >= 4)
    {
        mortalWounds++;
    }
    roll = dice.rollD6();
    if (roll >= 4)
    {
        mortalWounds++;
    }

    return mortalWounds;
}

Unit *EvocatorsOnCelestialDracolines::Create(const ParameterList &parameters)
{
    auto *evos = new EvocatorsOnCelestialDracolines();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool primeGrandstave = GetBoolParam("Prime Grandstave", parameters, false);
    int numGrandstaves = GetIntParam("Grandstaves", parameters, 0);

    bool ok = evos->configure(numModels, numGrandstaves, primeGrandstave);
    if (!ok)
    {
        delete evos;
        evos = nullptr;
    }
    return evos;
}

void EvocatorsOnCelestialDracolines::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Evocators on Celestial Dracolines", factoryMethod);
    }
}

void EvocatorsOnCelestialDracolines::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_tempestBladeAndStave);
    visitor(&m_tempestBladeAndStavePrime);
    visitor(&m_grandStave);
    visitor(&m_grandStavePrime);
    visitor(&m_monstrousClaws);
}

Rerolls EvocatorsOnCelestialDracolines::chargeRerolls() const
{
    if (m_charged)
        return RerollFailed;
    return StormcastEternal::chargeRerolls();
}

} // namespace StormcastEternals
