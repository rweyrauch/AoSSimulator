/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/VanguardRaptorsLongstrike.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{

bool VanguardRaptorsLongstrike::s_registered = false;

VanguardRaptorsLongstrike::VanguardRaptorsLongstrike() :
    StormcastEternal("Vanguard Raptors with Longstrike Crossbows", 5, WOUNDS, 7, 4, false),
    m_longstikeCrossbow(Weapon::Type::Missile, "Longstrike Crossbow", 24, 1, 2, 3, -2, 2),
    m_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 1, 4, 3, 0, 1),
    m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, VANGUARD_RAPTORS};
    m_weapons = {&m_longstikeCrossbow, &m_heavyStock, &m_beakAndClaws};
}

bool VanguardRaptorsLongstrike::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Prime
    auto primeModel = new Model(BASESIZE, WOUNDS);
    primeModel->addMissileWeapon(&m_longstikeCrossbow);
    primeModel->addMeleeWeapon(&m_heavyStock);
    primeModel->addMeleeWeapon(&m_beakAndClaws);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_longstikeCrossbow);
        model->addMeleeWeapon(&m_heavyStock);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

void VanguardRaptorsLongstrike::onStartShooting(PlayerId player)
{
    if (player == owningPlayer())
    {
        // Longshot
        if (!m_moved)
        {
            m_longstikeCrossbow.setRange(m_longstikeCrossbow.range() + 6);
        }
        else
        {
            m_longstikeCrossbow.setRange(m_longstikeCrossbow.range());
        }
    }
    StormcastEternal::onStartShooting(player);
}

Unit *VanguardRaptorsLongstrike::Create(const ParameterList &parameters)
{
    auto unit = new VanguardRaptorsLongstrike();
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

void VanguardRaptorsLongstrike::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            StormcastEternal::ValueToString,
            StormcastEternal::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Vanguard Raptors with Longstrike Crossbows", factoryMethod);
    }
}

Wounds VanguardRaptorsLongstrike::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Headshot
    if ((hitRoll == 6) && (weapon->name() == m_longstikeCrossbow.name()))
    {
        return {0, 2};
    }
    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int VanguardRaptorsLongstrike::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace StormcastEternals
