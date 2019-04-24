/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Protectors.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Protectors::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    {
        {ParamType::Integer, "Models", Protectors::MIN_UNIT_SIZE, Protectors::MIN_UNIT_SIZE, Protectors::MAX_UNIT_SIZE, Protectors::MIN_UNIT_SIZE},
        {ParamType::Integer, "Starsoul Maces", 2, 0, (Protectors::MAX_UNIT_SIZE / 5) * 2, 1},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Protectors::s_registered = false;

Protectors::Protectors() :
    StormcastEternal("Protectors", 4, WOUNDS, 7, 4, false),
    m_glaive(Weapon::Type::Melee, "Stormstrike Glaive", 3, 3, 3, 3, -1, 1),
    m_glaivePrime(Weapon::Type::Melee, "Stormstrike Glaive (Prime)", 3, 4, 3, 3, -1, 1),
    m_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 1, 0, 0, 0, 0)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, PROTECTORS};
}

bool Protectors::configure(int numModels, int numStarsoulMaces)
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
    Model primeModel(BASESIZE, WOUNDS);
    primeModel.addMeleeWeapon(&m_glaivePrime);
    addModel(primeModel);

    for (auto i = 0; i < numStarsoulMaces; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_starsoulMace);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_glaive);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Wounds Protectors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Deathstrike
    if ((hitRoll == 6) && target->hasKeyword(MONSTER) && (weapon->name() == m_glaive.name()))
    {
        Dice dice;
        return {dice.rollD6(), 0};
    }
    // Starsoul Mace
    if (weapon->name() == m_starsoulMace.name())
    {
        int mortalWounds = 0;
        Dice dice;
        int roll = dice.rollD6();
        if (roll >= 6)
        {
            mortalWounds = dice.rollD3() + 1;
        }
        else if (roll >= 2)
        {
            mortalWounds = dice.rollD3();
        }
        return {0, mortalWounds};
    }
    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Unit *Protectors::Create(const ParameterList &parameters)
{
    auto unit = new Protectors();
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

void Protectors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Protectors", factoryMethod);
    }
}

void Protectors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_glaive);
    visitor(&m_glaivePrime);
    visitor(&m_starsoulMace);
}

int Protectors::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = StormcastEternal::targetHitModifier(weapon, attacker);

    // Storm-shield
    if (weapon->isMissile())
    {
        modifier -= 1;
    }
    return modifier;
}

} // namespace StormcastEternals
