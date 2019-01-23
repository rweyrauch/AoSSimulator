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
    {
        {ParamType::Integer, "numModels", {.m_intValue = 5}, VanguardHunters::MIN_UNIT_SIZE, VanguardHunters::MAX_UNIT_SIZE, VanguardHunters::MIN_UNIT_SIZE},
        {ParamType::Integer, "weapons", {.m_intValue = VanguardHunters::StormSabre}, VanguardHunters::ShockHandaxe,
         VanguardHunters::StormSabre, 1},
        {ParamType::Boolean, "astralCompass", {.m_boolValue = false}, false, false},
    }
};

bool VanguardHunters::s_registered = false;

Weapon VanguardHunters::s_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 4, 0, 1);
Weapon VanguardHunters::s_boltstormPistolPrime(Weapon::Type::Missile, "Boltstorm Pistol (Prime)", 9, 3, 3, 4, 0, 1);
Weapon VanguardHunters::s_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 4, 3, 0, 1);
Weapon VanguardHunters::s_shockHandaxePrime(Weapon::Type::Melee, "Shock Handaxe (Prime)", 1, 3, 4, 3, 0, 1);
Weapon VanguardHunters::s_stormSabre(Weapon::Type::Melee, "Storm Sabre", 1, 2, 3, 4, 0, 1);
Weapon VanguardHunters::s_stormSabrePrime(Weapon::Type::Melee, "Storm Sabre (Prime)", 1, 3, 3, 4, 0, 1);


VanguardHunters::VanguardHunters() :
    StormcastEternal("Vanguard-Hunters", 6, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, ANGELOS, VANGUARD_HUNTERS };

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
        primeModel.addMeleeWeapon(&s_stormSabrePrime);
    }
    else if (m_weaponOption == ShockHandaxe)
    {
        primeModel.addMeleeWeapon(&s_shockHandaxePrime);
    }
    primeModel.addMissileWeapon(&s_boltstormPistolPrime);
    addModel(primeModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == StormSabre)
            model.addMeleeWeapon(&s_stormSabre);
        else if (m_weaponOption == ShockHandaxe)
            model.addMeleeWeapon(&s_shockHandaxe);
        model.addMissileWeapon(&s_shockHandaxePrime);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_boltstormPistol.name() << ": " << s_boltstormPistol.strength() << std::endl;
        std::cout << "\t" << s_boltstormPistolPrime.name() << ": " << s_boltstormPistolPrime.strength() << std::endl;
        std::cout << "\t" << s_shockHandaxe.name() << ": " << s_shockHandaxe.strength() << std::endl;
        std::cout << "\t" << s_shockHandaxePrime.name() << ": " << s_shockHandaxePrime.strength() << std::endl;
        std::cout << "\t" << s_stormSabre.name() << ": " << s_stormSabre.strength() << std::endl;
        std::cout << "\t" << s_stormSabrePrime.name() << ": " << s_stormSabrePrime.strength() << std::endl;
    }

    return true;
}

Unit *VanguardHunters::Create(const ParameterList &parameters)
{
    auto hunters = new VanguardHunters();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption)GetIntParam("weapons", parameters, StormSabre);
    int astralCompass = GetBoolParam("astralCompass", parameters, false);

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
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == StormSabre)
            return "StormSabre";
        else if (parameter.m_intValue == ShockHandaxe)
            return "ShockHandaxe";
    }
    return ParameterValueToString(parameter);
}

} // namespace StormcastEternals
