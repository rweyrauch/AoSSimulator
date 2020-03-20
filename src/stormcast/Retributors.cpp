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

bool Retributors::s_registered = false;

Retributors::Retributors() :
    StormcastEternal("Retributors", 4, WOUNDS, 7, 4, false),
    m_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 2, 3, 3, -1, 2),
    m_lightningHammerPrime(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2),
    m_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 0, 0, 0, 0, 0)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, RETRIBUTORS};
    m_weapons = {&m_lightningHammer, &m_lightningHammerPrime, &m_starsoulMace};
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
    auto primeModel = new Model(BASESIZE, wounds());
    primeModel->addMeleeWeapon(&m_lightningHammerPrime);
    addModel(primeModel);

    for (auto i = 0; i < numStarsoulMaces; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_starsoulMace);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_lightningHammer);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Wounds Retributors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Blast to Ashes
    if ((hitRoll == 6) && (weapon->name() == m_lightningHammer.name()))
    {
        return {0, 2};
    }

    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Unit *Retributors::Create(const ParameterList &parameters)
{
    auto unit = new Retributors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numStarsoulMaces = GetIntParam("Starsoul Maces", parameters, 0);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

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
        static FactoryMethod factoryMethod = {
            Create,
            StormcastEternal::ValueToString,
            StormcastEternal::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Integer, "Starsoul Maces", 2, 0, (MAX_UNIT_SIZE / 5) * 2, 1},
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Retributors", factoryMethod);
    }
}

void Retributors::onStartCombat(PlayerId player)
{
    Unit::onStartCombat(player);

    if (m_meleeTarget)
    {
        for (const auto& ip : m_models)
        {
            // Starsoul Mace
            if (ip->preferredWeapon()->name() == m_starsoulMace.name())
            {
                int mortalWounds = 0;
                int roll = Dice::rollD6();
                if (roll >= 6)
                {
                    mortalWounds = Dice::rollD3() + 1;
                }
                else if (roll >= 2)
                {
                    mortalWounds = Dice::rollD3();
                }

                m_meleeTarget->applyDamage({0, mortalWounds});
            }
        }
    }
}

int Retributors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace StormcastEternals
