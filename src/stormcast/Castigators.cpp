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

Weapon Castigators::s_thunderheadGreatbow(Weapon::Type::Missile, "Thunderhead Greatbow", 18, 1, 3, 3, -1, 1);
Weapon Castigators::s_thunderheadGreatbowPrime(Weapon::Type::Missile, "Thunderhead Greatbow (Prime)", 18, 1, 2, 3, -1, 1);
Weapon Castigators::s_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 2, 4, 4, 0, 1);

Castigators::Castigators() :
    StormcastEternal("Castigators", 5, WOUNDS, 7, 4, false)
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

    m_pThunderheadGreatbowPrime = new Weapon(s_thunderheadGreatbowPrime);
    m_pThunderheadGreatbow = new Weapon(s_thunderheadGreatbow);

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    primeModel.addMissileWeapon(m_pThunderheadGreatbowPrime);
    primeModel.addMeleeWeapon(&s_heavyStock);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(m_pThunderheadGreatbow);
        model.addMeleeWeapon(&s_heavyStock);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_thunderheadGreatbow.name() << ": " << s_thunderheadGreatbow.strength() << std::endl;
        std::cout << "\t" << s_thunderheadGreatbowPrime.name() << ": " << s_thunderheadGreatbowPrime.strength() << std::endl;
        std::cout << "\t" << s_heavyStock.name() << ": " << s_heavyStock.strength() << std::endl;
    }

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
    if (weapon->name() == s_thunderheadGreatbow.name())
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
        m_pThunderheadGreatbow->setRend(-2);
        m_pThunderheadGreatbowPrime->setRend(-2);
    }
    else
    {
        m_pThunderheadGreatbow->setRend(s_thunderheadGreatbow.rend());
        m_pThunderheadGreatbowPrime->setRend(s_thunderheadGreatbowPrime.rend());
    }

    Unit::onStartShooting(player);
}

Rerolls Castigators::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    if (weapon->name() == s_thunderheadGreatbow.name())
    {
        if (!m_aethericChannellingPower)
            return RerollOnes;
    }
    return StormcastEternal::toHitRerolls(weapon, unit);
}

} // namespace StormcastEternals
