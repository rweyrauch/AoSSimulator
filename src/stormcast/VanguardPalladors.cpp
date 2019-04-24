/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/VanguardPalladors.h>
#include <iostream>

namespace StormcastEternals
{

static FactoryMethod factoryMethod = {
    VanguardPalladors::Create,
    VanguardPalladors::ValueToString,
    VanguardPalladors::EnumStringToInt,
    {
        {ParamType::Integer, "Models", VanguardPalladors::MIN_UNIT_SIZE, VanguardPalladors::MIN_UNIT_SIZE,
         VanguardPalladors::MAX_UNIT_SIZE, VanguardPalladors::MIN_UNIT_SIZE},
        {
            ParamType::Enum, "Weapons", VanguardPalladors::StarstrikeJavelin, VanguardPalladors::ShockHandaxe, VanguardPalladors::StarstrikeJavelin, 1
        },
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool VanguardPalladors::s_registered = false;

VanguardPalladors::VanguardPalladors() :
    StormcastEternal("Vanguard-Palladors", 12, WOUNDS, 7, 4, false),
    m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 4, 0, 1),
    m_starstrikeJavelinMissile(Weapon::Type::Missile, "Starstrike Javelin", 18, 1, 3, 3, -1, 1),
    m_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 3, 3, 0, 1),
    m_starstrikeJavelin(Weapon::Type::Melee, "Starstrike Javelin", 2, 1, 4, 3, -1, 1),
    m_beakAndClaws(Weapon::Type::Melee, "Razor Beak and Claws", 1, 3, 3, 3, -2, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, GRYPH_CHARGER, STORMCAST_ETERNAL, ANGELOS, VANGUARD_PALLADORS};
}

bool VanguardPalladors::configure(int numModels, WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (weapons == ShockHandaxe)
    {
        primeModel.addMeleeWeapon(&m_shockHandaxe);
    }
    else
    {
        primeModel.addMissileWeapon(&m_starstrikeJavelinMissile);
        primeModel.addMeleeWeapon(&m_starstrikeJavelin);
    }
    primeModel.addMissileWeapon(&m_boltstormPistol);
    primeModel.addMeleeWeapon(&m_beakAndClaws);
    addModel(primeModel);

    auto currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == ShockHandaxe)
        {
            model.addMeleeWeapon(&m_shockHandaxe);
        }
        else
        {
            model.addMissileWeapon(&m_starstrikeJavelinMissile);
            model.addMeleeWeapon(&m_starstrikeJavelin);
        }
        model.addMissileWeapon(&m_boltstormPistol);
        model.addMeleeWeapon(&m_beakAndClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *VanguardPalladors::Create(const ParameterList &parameters)
{
    auto *unit = new VanguardPalladors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, StarstrikeJavelin);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VanguardPalladors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Vanguard-Palladors", factoryMethod);
    }
}

void VanguardPalladors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_boltstormPistol);
    visitor(&m_starstrikeJavelinMissile);
    visitor(&m_shockHandaxe);
    visitor(&m_starstrikeJavelin);
    visitor(&m_beakAndClaws);
}

std::string VanguardPalladors::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == StarstrikeJavelin)
        {
            return "Starstrike Javelin";
        }
        else if (parameter.m_intValue == ShockHandaxe)
        {
            return "Shock Handaxe";
        }
    }
    return StormcastEternal::ValueToString(parameter);
}

int VanguardPalladors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Starstrike Javelin")
    {
        return StarstrikeJavelin;
    }
    else if (enumString == "Shock Handaxe")
    {
        return ShockHandaxe;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

void VanguardPalladors::onEndCombat(PlayerId player)
{
    Unit::onEndCombat(player);

    // TODO: make sure prime model exists
    // Lunar Blade
    if (distanceTo(m_meleeTarget) <= 1.0f)
    {
        Dice dice;
        int roll = dice.rollD6();
        if (roll >= 2)
        {
            m_meleeTarget->applyDamage({0, 1});
        }
    }
}

Wounds VanguardPalladors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Aethereal Strike
    if ((hitRoll == 6) && (weapon->name() == m_beakAndClaws.name()))
    {
        return {0, 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace StormcastEternals
