/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <stormcast/SteelheartsChampions.h>

namespace StormcastEternals
{

static FactoryMethod factoryMethod = {
    SteelheartsChampions::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    SteelheartsChampions::ComputePoints,
    {
    },
    ORDER,
    { STORMCAST_ETERNAL }
};

bool SteelheartsChampions::s_registered = false;

SteelheartsChampions::SteelheartsChampions() :
    StormcastEternal("Steelheart's Champions", 5, WOUNDS, 7, 4, false),
    m_broadSword(Weapon::Type::Melee, "Serevin's Broadsword", 1, 3, 3, 4, -1, 2),
    m_grandhammer(Weapon::Type::Melee, "Obryn's Grandhammer", 1, 2, 4, 3, -1, 3),
    m_warhammer(Weapon::Type::Melee, "Angharad's Warhammer", 1, 3, 3, 3, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, REDEEMER, LIBERATORS, STEELHEARTS_CHAMPIONS};
    m_weapons = {&m_broadSword, &m_grandhammer, &m_warhammer};
}

bool SteelheartsChampions::configure()
{
    auto severin = new Model(BASESIZE, WOUNDS);
    severin->setName("Severin");
    severin->addMeleeWeapon(&m_broadSword);
    addModel(severin);

    auto obryn = new Model(BASESIZE, WOUNDS);
    obryn->setName("Obryn");
    obryn->addMeleeWeapon(&m_grandhammer);
    addModel(obryn);

    auto angharad = new Model(BASESIZE, WOUNDS);
    angharad->setName("Angharad");
    angharad->addMeleeWeapon(&m_warhammer);
    addModel(angharad);

    m_points = POINTS_PER_UNIT;

    return true;
}

int SteelheartsChampions::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = StormcastEternal::toHitModifier(weapon, unit);

    // Lay Low the Tyrants
    if (unit->wounds() >= 5)
    {
        modifier += 1;
    }

    return modifier;
}

Rerolls SteelheartsChampions::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shields
    for (const auto& ip : m_models)
    {
        if (ip->getName() == "Angharad")
        {
            return RerollOnes;
        }
    }

    return StormcastEternal::toSaveRerolls(weapon);
}

Unit *SteelheartsChampions::Create(const ParameterList &parameters)
{
    auto unit = new SteelheartsChampions();
    unit->setStormhost(HammersOfSigmar);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SteelheartsChampions::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Steelheart's Champions", factoryMethod);
    }
}

Rerolls SteelheartsChampions::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if (weapon->name() == m_broadSword.name() && target->remainingModels() >= 5)
    {
        return RerollFailed;
    }
    return StormcastEternal::toHitRerolls(weapon, target);
}

} // namespace StormcastEternals
