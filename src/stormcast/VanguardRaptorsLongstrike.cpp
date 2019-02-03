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
static FactoryMethod factoryMethod = {
    VanguardRaptorsLongstrike::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = 3}, VanguardRaptorsLongstrike::MIN_UNIT_SIZE,
         VanguardRaptorsLongstrike::MAX_UNIT_SIZE, VanguardRaptorsLongstrike::MIN_UNIT_SIZE},
    }
};

bool VanguardRaptorsLongstrike::s_registered = false;

Weapon VanguardRaptorsLongstrike::s_longstikeCrossbow(Weapon::Type::Missile, "Longstrike Crossbow", 24, 1, 2, 3, -2, 2);
Weapon VanguardRaptorsLongstrike::s_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 1, 4, 3, 0, 1);
Weapon VanguardRaptorsLongstrike::s_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 4, 3, 0, 1);

VanguardRaptorsLongstrike::VanguardRaptorsLongstrike() :
    StormcastEternal("Vanguard Raptors with Longstrike Crossbows", 5, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, VANGUARD_RAPTORS };
}

bool VanguardRaptorsLongstrike::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_pLongstikeCrossbow = new Weapon(s_longstikeCrossbow);

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    primeModel.addMissileWeapon(m_pLongstikeCrossbow);
    primeModel.addMeleeWeapon(&s_heavyStock);
    primeModel.addMeleeWeapon(&s_beakAndClaws);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(m_pLongstikeCrossbow);
        model.addMeleeWeapon(&s_heavyStock);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void VanguardRaptorsLongstrike::onStartShooting(PlayerId player)
{
    if (player == m_owningPlayer)
    {
        // Longshot
        if (!m_moved)
        {
            m_pLongstikeCrossbow->setRange(s_longstikeCrossbow.range() + 6);
        }
        else
        {
            m_pLongstikeCrossbow->setRange(s_longstikeCrossbow.range());
        }
    }
    StormcastEternal::onStartShooting(player);
}

Unit *VanguardRaptorsLongstrike::Create(const ParameterList &parameters)
{
    auto unit = new VanguardRaptorsLongstrike();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

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
        s_registered = UnitFactory::Register("VanguardRaptorsLongstrike", factoryMethod);
    }
}

int VanguardRaptorsLongstrike::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits, const WoundingHits& wounds)
{
    // Headshot
    if (weapon->name() == s_longstikeCrossbow.name())
    {
        int mortalWounds = hits.rolls.numUnmodified6s() * 2;
        return mortalWounds;
    }
    return StormcastEternal::generateMortalWounds(weapon, unit, hits, wounds);
}

void VanguardRaptorsLongstrike::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_longstikeCrossbow);
    visitor(&s_heavyStock);
    visitor(&s_beakAndClaws);
}

} // namespace StormcastEternals
