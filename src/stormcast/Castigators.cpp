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

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    primeModel.addMissileWeapon(&s_thunderheadGreatbowPrime);
    primeModel.addMeleeWeapon(&s_heavyStock);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&s_thunderheadGreatbow);
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

} // namespace StormcastEternals
