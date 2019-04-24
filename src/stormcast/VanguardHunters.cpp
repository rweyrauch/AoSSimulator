/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/VanguardHunters.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    VanguardHunters::Create,
    VanguardHunters::ValueToString,
    VanguardHunters::EnumStringToInt,
    {
        {ParamType::Integer, "Models", VanguardHunters::MIN_UNIT_SIZE, VanguardHunters::MIN_UNIT_SIZE, VanguardHunters::MAX_UNIT_SIZE, VanguardHunters::MIN_UNIT_SIZE},
        {
            ParamType::Enum, "Weapons", VanguardHunters::StormSabre, VanguardHunters::ShockHandaxe, VanguardHunters::StormSabre, 1
        },
        {ParamType::Boolean, "Astral Compass", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool VanguardHunters::s_registered = false;

VanguardHunters::VanguardHunters() :
    StormcastEternal("Vanguard-Hunters", 6, WOUNDS, 7, 4, false),
    m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 4, 0, 1),
    m_boltstormPistolPrime(Weapon::Type::Missile, "Boltstorm Pistol (Prime)", 9, 3, 3, 4, 0, 1),
    m_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 4, 3, 0, 1),
    m_shockHandaxePrime(Weapon::Type::Melee, "Shock Handaxe (Prime)", 1, 3, 4, 3, 0, 1),
    m_stormSabre(Weapon::Type::Melee, "Storm Sabre", 1, 2, 3, 4, 0, 1),
    m_stormSabrePrime(Weapon::Type::Melee, "Storm Sabre (Prime)", 1, 3, 3, 4, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, ANGELOS, VANGUARD_HUNTERS};

    // Tireless Hunters
    m_runAndShoot = true;
}

bool VanguardHunters::configure(int numModels, WeaponOption weapons, bool astralCompass)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_weaponOption = weapons;
    m_astralCompass = astralCompass;

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (m_weaponOption == StormSabre)
    {
        primeModel.addMeleeWeapon(&m_stormSabrePrime);
    }
    else if (m_weaponOption == ShockHandaxe)
    {
        primeModel.addMeleeWeapon(&m_shockHandaxePrime);
    }
    primeModel.addMissileWeapon(&m_boltstormPistolPrime);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == StormSabre)
        {
            model.addMeleeWeapon(&m_stormSabre);
        }
        else if (m_weaponOption == ShockHandaxe)
        {
            model.addMeleeWeapon(&m_shockHandaxe);
        }
        model.addMissileWeapon(&m_shockHandaxePrime);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *VanguardHunters::Create(const ParameterList &parameters)
{
    auto hunters = new VanguardHunters();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, StormSabre);
    bool astralCompass = GetBoolParam("Astral Compass", parameters, false);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    hunters->setStormhost(stormhost);

    bool ok = hunters->configure(numModels, weapons, astralCompass);
    if (!ok)
    {
        delete hunters;
        hunters = nullptr;
    }
    return hunters;
}

void VanguardHunters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Vanguard-Hunters", factoryMethod);
    }
}

std::string VanguardHunters::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == StormSabre)
        {
            return "Storm Sabre";
        }
        else if (parameter.m_intValue == ShockHandaxe)
        {
            return "Shock Handaxe";
        }
    }
    return StormcastEternal::ValueToString(parameter);
}

int VanguardHunters::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Storm Sabre")
    {
        return StormSabre;
    }
    else if (enumString == "Shock Handaxe")
    {
        return ShockHandaxe;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

void VanguardHunters::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_boltstormPistol);
    visitor(&m_boltstormPistolPrime);
    visitor(&m_shockHandaxe);
    visitor(&m_shockHandaxePrime);
    visitor(&m_stormSabre);
    visitor(&m_stormSabrePrime);
}

} // namespace StormcastEternals
