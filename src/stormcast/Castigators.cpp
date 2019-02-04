/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Castigators.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Castigators::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = 3}, Castigators::MIN_UNIT_SIZE, Castigators::MAX_UNIT_SIZE, Castigators::MIN_UNIT_SIZE},
    }
};

bool Castigators::s_registered = false;

Castigators::Castigators() :
    StormcastEternal("Castigators", 5, WOUNDS, 7, 4, false),
    m_thunderheadGreatbow(Weapon::Type::Missile, "Thunderhead Greatbow", 18, 1, 3, 3, -1, 1),
    m_thunderheadGreatbowPrime(Weapon::Type::Missile, "Thunderhead Greatbow (Prime)", 18, 1, 2, 3, -1, 1),
    m_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 2, 4, 4, 0, 1)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, JUSTICAR, CASTIGATORS };
}

bool Castigators::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    primeModel.addMissileWeapon(&m_thunderheadGreatbowPrime);
    primeModel.addMeleeWeapon(&m_heavyStock);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_thunderheadGreatbow);
        model.addMeleeWeapon(&m_heavyStock);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

Unit *Castigators::Create(const ParameterList &parameters)
{
    auto unit = new Castigators();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Castigators::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Castigators", factoryMethod);
    }
}

Hits Castigators::applyHitModifiers(const Weapon *weapon, const Unit *unit, const Hits &hits) const
{
    // Burst of Celestial Energy
    if (weapon->name() == m_thunderheadGreatbow.name())
    {
        Hits modHits = hits;
        if (unit->hasKeyword(DAEMON) || unit->hasKeyword(NIGHTHAUNT))
        {
            Dice dice;
            int num6s = hits.rolls.numUnmodified6s();
            for (int i = 0; i < num6s; i++)
            {
                modHits.numHits += (dice.rollD3() - 1); // -1 to replace initial hit
            }
        }
        return modHits;
    }
    return Unit::applyHitModifiers(weapon, unit, hits);
}

void Castigators::onStartShooting(PlayerId player)
{
    if (player == m_owningPlayer)
    {
        // Aetheric Channelling
        if (m_shootingTarget)
        {
            if (m_shootingTarget->ignoreRend())
                m_aethericChannellingPower = false;
            else if (m_shootingTarget->save() < 4)
                m_aethericChannellingPower = true;
            else // accuracy
                m_aethericChannellingPower = false;
        }
    }

    if (m_aethericChannellingPower)
    {
        m_thunderheadGreatbow.setRend(-2);
        m_thunderheadGreatbowPrime.setRend(-2);
    }
    else
    {
        m_thunderheadGreatbow.setRend(m_thunderheadGreatbow.rend());
        m_thunderheadGreatbowPrime.setRend(m_thunderheadGreatbowPrime.rend());
    }

    Unit::onStartShooting(player);
}

Rerolls Castigators::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    if (weapon->name() == m_thunderheadGreatbow.name())
    {
        if (!m_aethericChannellingPower)
            return RerollOnes;
    }
    return StormcastEternal::toHitRerolls(weapon, unit);
}

void Castigators::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_thunderheadGreatbow);
    visitor(&m_thunderheadGreatbowPrime);
    visitor(&m_heavyStock);
}

} // namespace StormcastEternals
