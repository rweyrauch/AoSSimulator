/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Judicators.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Judicators::Create,
    Judicators::ValueToString,
    Judicators::EnumStringToInt,
    {
        {ParamType::Integer, "numModels", {.m_intValue = 5}, Judicators::MIN_UNIT_SIZE, Judicators::MAX_UNIT_SIZE, Judicators::MIN_UNIT_SIZE},
        {
            ParamType::Integer, "weapons", {.m_intValue = Judicators::SkyboltBow}, Judicators::SkyboltBow,
            Judicators::BoltstormCrossbow, 1
        },
        {ParamType::Integer, "numShockboltBows", {.m_intValue = 1}, 0, Judicators::MAX_UNIT_SIZE / 5},
        {ParamType::Integer, "numThunderboltCrossbows", {.m_intValue = 0}, 0, Judicators::MAX_UNIT_SIZE / 5}
    }
};

bool Judicators::s_registered = false;

Judicators::Judicators() :
    StormcastEternal("Judicators", 5, WOUNDS, 7, 4, false),
    m_skyboltBow(Weapon::Type::Missile, "Skybolt Bow", 24, 1, 3, 3, -1, 1),
    m_skyboltPrime(Weapon::Type::Missile, "Skybolt Bow (Prime)", 24, 1, 2, 3, -1, 1),
    m_boltstormCrossbow(Weapon::Type::Missile, "Boltstorm Crossbow", 12, 3, 3, 4, 0, 1),
    m_shockboltBow(Weapon::Type::Missile, "Shockbolt Bow", 24, 1, 3, 3, -1, 1),
    m_shockboltPrime(Weapon::Type::Missile, "Shockbolt Bow (Prime)", 24, 1, 2, 3, -1, 1),
    m_thunderboldCrossbow(Weapon::Type::Missile, "Thunderbolt Crossbow", 18, 0, 0, 0, 0, 0),
    m_stormGladius(Weapon::Type::Melee, "Storm Gladius", 1, 1, 3, 4, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, JUDICATORS};

    // Chained Lightning
    m_shockboltBow.setHitsPerAttack(RAND_D6);
}

bool Judicators::configure(int numModels, WeaponOption weapons, int numShockboltBows, int numThunderboltCrossbows)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxSpecialWeapons = numModels / 5;
    if (numShockboltBows + numThunderboltCrossbows > maxSpecialWeapons)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_weaponOption = weapons;

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (m_weaponOption == SkyboltBow)
    {
        primeModel.addMissileWeapon(&m_skyboltPrime);
    }
    else if (m_weaponOption == BoltstormCrossbow)
    {
        primeModel.addMissileWeapon(&m_shockboltPrime);
    }
    primeModel.addMeleeWeapon(&m_stormGladius);
    addModel(primeModel);

    for (auto i = 0; i < numShockboltBows; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_shockboltBow);
        model.addMeleeWeapon(&m_stormGladius);
        addModel(model);
    }
    for (auto i = 0; i < numThunderboltCrossbows; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_thunderboldCrossbow);
        model.addMeleeWeapon(&m_stormGladius);
        addModel(model);
    }
    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == SkyboltBow)
        {
            model.addMissileWeapon(&m_skyboltBow);
        }
        else if (m_weaponOption == BoltstormCrossbow)
        {
            model.addMissileWeapon(&m_boltstormCrossbow);
        }
        model.addMeleeWeapon(&m_stormGladius);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Rerolls Judicators::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // External Judgement
    if (unit->hasKeyword(CHAOS) && weapon->isMissile())
    {
        return RerollOnes;
    }
    return StormcastEternal::toHitRerolls(weapon, unit);
}

int Judicators::extraAttacks(const Weapon *weapon) const
{
    // Rapid Fire
    if (!m_ran && weapon->isMissile())
    {
        return 1;
    }
    return StormcastEternal::extraAttacks(weapon);
}

Unit *Judicators::Create(const ParameterList &parameters)
{
    auto juds = new Judicators();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetIntParam("weapons", parameters, SkyboltBow);
    int numShockboltBows = GetIntParam("numShockboltBows", parameters, 0);
    int numThunderboltCrossbows = GetIntParam("numThunderboltCrossbows", parameters, 0);

    bool ok = juds->configure(numModels, weapons, numShockboltBows, numThunderboltCrossbows);
    if (!ok)
    {
        delete juds;
        juds = nullptr;
    }
    return juds;
}

void Judicators::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Judicators", factoryMethod);
    }
}

std::string Judicators::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == SkyboltBow)
        {
            return "SkyboltBow";
        }
        else if (parameter.m_intValue == BoltstormCrossbow)
        {
            return "BoltstormCrossbow";
        }
    }
    return ParameterValueToString(parameter);
}

int Judicators::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "SkyboltBow")
    {
        return SkyboltBow;
    }
    else if (enumString == "BoltstormCrossbow")
    {
        return BoltstormCrossbow;
    }
    return 0;
}

void Judicators::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_stormGladius);
    visitor(&m_shockboltBow);
    visitor(&m_shockboltPrime);
    visitor(&m_skyboltBow);
    visitor(&m_skyboltPrime);
    visitor(&m_thunderboldCrossbow);
    visitor(&m_boltstormCrossbow);
}

} // namespace StormcastEternals
