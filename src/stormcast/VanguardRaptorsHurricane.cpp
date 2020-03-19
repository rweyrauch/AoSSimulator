/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/VanguardRaptorsHurricane.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{

bool VanguardRaptorsHurricane::s_registered = false;

VanguardRaptorsHurricane::VanguardRaptorsHurricane() :
    StormcastEternal("Vanguard Raptors with Hurricane Crossbows", 5, WOUNDS, 7, 4, false),
    m_hurricaneCrossbow(Weapon::Type::Missile, "Hurricane Crossbow", 18, 6, 4, 4, 0, 1),
    m_hurricaneCrossbowPrime(Weapon::Type::Missile, "Hurricane Crossbow", 18, 6, 3, 4, 0, 1),
    m_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, VANGUARD_RAPTORS};
    m_weapons = {&m_hurricaneCrossbow, &m_hurricaneCrossbowPrime, &m_heavyStock};
}

bool VanguardRaptorsHurricane::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Prime
    auto primeModel = new Model(BASESIZE, WOUNDS);
    primeModel->addMissileWeapon(&m_hurricaneCrossbowPrime);
    primeModel->addMeleeWeapon(&m_heavyStock);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_hurricaneCrossbow);
        model->addMeleeWeapon(&m_heavyStock);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *VanguardRaptorsHurricane::Create(const ParameterList &parameters)
{
    auto unit = new VanguardRaptorsHurricane();
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

void VanguardRaptorsHurricane::Init()
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

        s_registered = UnitFactory::Register("Vanguard Raptors with Hurricane Crossbows", factoryMethod);
    }
}

int VanguardRaptorsHurricane::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Rapid Fire
    if (!m_moved && weapon->name() == m_hurricaneCrossbow.name())
    {
        return 3;
    }
    return StormcastEternal::extraAttacks(nullptr, weapon, target);
}

int VanguardRaptorsHurricane::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace StormcastEternals
