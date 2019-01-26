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
static FactoryMethod factoryMethod = {
    VanguardRaptorsHurricane::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = 3}, VanguardRaptorsHurricane::MIN_UNIT_SIZE,
         VanguardRaptorsHurricane::MAX_UNIT_SIZE, VanguardRaptorsHurricane::MIN_UNIT_SIZE},
    }
};

bool VanguardRaptorsHurricane::s_registered = false;

Weapon VanguardRaptorsHurricane::s_hurricaneCrossbow(Weapon::Type::Missile, "Hurricane Crossbow", 18, 6, 4, 4, 0, 1);
Weapon VanguardRaptorsHurricane::s_hurricaneCrossbowPrime(Weapon::Type::Missile, "Hurricane Crossbow (Prime)", 18, 6, 3, 4, 0, 1);
Weapon VanguardRaptorsHurricane::s_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 1, 4, 4, 0, 1);

VanguardRaptorsHurricane::VanguardRaptorsHurricane() :
    StormcastEternal("Vanguard Raptors with Hurricane Crossbows", 5, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, VANGUARD_RAPTORS };
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
    Model primeModel(BASESIZE, WOUNDS);
    primeModel.addMissileWeapon(&s_hurricaneCrossbowPrime);
    primeModel.addMeleeWeapon(&s_heavyStock);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&s_hurricaneCrossbow);
        model.addMeleeWeapon(&s_heavyStock);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_hurricaneCrossbow.name() << ": " << s_hurricaneCrossbow.strength() << std::endl;
        std::cout << "\t" << s_hurricaneCrossbowPrime.name() << ": " << s_hurricaneCrossbowPrime.strength() << std::endl;
        std::cout << "\t" << s_heavyStock.name() << ": " << s_heavyStock.strength() << std::endl;
    }

    return true;
}

Unit *VanguardRaptorsHurricane::Create(const ParameterList &parameters)
{
    auto unit = new VanguardRaptorsHurricane();
    int numModels = GetIntParam("VanguardRaptorsHurricane", parameters, MIN_UNIT_SIZE);

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
        s_registered = UnitFactory::Register("VanguardRaptorsHurricane", factoryMethod);
    }
}

int VanguardRaptorsHurricane::extraAttacks(const Weapon *weapon) const
{
    // Rapid Fire
    if (!m_moved && weapon->name() == s_hurricaneCrossbow.name())
    {
        return 3;
    }
    return StormcastEternal::extraAttacks(weapon);
}

} // namespace StormcastEternals
