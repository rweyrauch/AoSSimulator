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

bool Castigators::s_registered = false;

Castigators::Castigators() :
    StormcastEternal("Castigators", 5, WOUNDS, 7, 4, false),
    m_thunderheadGreatbow(Weapon::Type::Missile, "Thunderhead Greatbow", 18, 1, 3, 3, -1, 1),
    m_thunderheadGreatbowPrime(Weapon::Type::Missile, "Thunderhead Greatbow", 18, 1, 2, 3, -1, 1),
    m_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, JUSTICAR, CASTIGATORS};
    m_weapons = {&m_thunderheadGreatbow, &m_thunderheadGreatbowPrime, &m_heavyStock};
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
    auto primeModel = new Model(BASESIZE, WOUNDS);
    primeModel->addMissileWeapon(&m_thunderheadGreatbowPrime);
    primeModel->addMeleeWeapon(&m_heavyStock);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_thunderheadGreatbow);
        model->addMeleeWeapon(&m_heavyStock);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Castigators::Create(const ParameterList &parameters)
{
    auto unit = new Castigators();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

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
        static FactoryMethod factoryMethod = {
            Create,
            StormcastEternal::ValueToString,
            StormcastEternal::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Castigators", factoryMethod);
    }
}

int Castigators::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Burst of Celestial Energy
    if (unmodifiedHitRoll == 6)
    {
        if (weapon->name() == m_thunderheadGreatbow.name())
        {
            if (unit->hasKeyword(DAEMON) || unit->hasKeyword(NIGHTHAUNT))
            {
                return Dice::rollD3();
            }
        }
    }
    return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
}

void Castigators::onStartShooting(PlayerId player)
{
    if (player == owningPlayer())
    {
        // Aetheric Channelling
        if (m_shootingTarget)
        {
            if (m_shootingTarget->ignoreRend())
                m_aethericChannellingPower = false;
            else
                m_aethericChannellingPower = (m_shootingTarget->save() < 4);
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

    StormcastEternal::onStartShooting(player);
}

Rerolls Castigators::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    if (weapon->name() == m_thunderheadGreatbow.name())
    {
        if (!m_aethericChannellingPower)
        {
            return RerollOnes;
        }
    }
    return StormcastEternal::toHitRerolls(weapon, unit);
}

int Castigators::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace StormcastEternals
