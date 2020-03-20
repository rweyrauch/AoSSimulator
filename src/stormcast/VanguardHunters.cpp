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

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 330;

    bool VanguardHunters::s_registered = false;

    VanguardHunters::VanguardHunters() :
            StormcastEternal("Vanguard-Hunters", 6, WOUNDS, 7, 4, false),
            m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 4, 0, 1),
            m_boltstormPistolPrime(Weapon::Type::Missile, "Boltstorm Pistol", 9, 3, 3, 4, 0, 1),
            m_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 4, 3, 0, 1),
            m_shockHandaxePrime(Weapon::Type::Melee, "Shock Handaxe", 1, 3, 4, 3, 0, 1),
            m_stormSabre(Weapon::Type::Melee, "Storm Sabre", 1, 2, 3, 4, 0, 1),
            m_stormSabrePrime(Weapon::Type::Melee, "Storm Sabre", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, ANGELOS, VANGUARD_HUNTERS};
        m_weapons = {&m_boltstormPistol, &m_boltstormPistolPrime, &m_shockHandaxe, &m_shockHandaxePrime, &m_stormSabre,
                     &m_stormSabrePrime};

        // Tireless Hunters
        m_runAndShoot = true;
    }

    bool VanguardHunters::configure(int numModels, WeaponOption weapons, bool astralCompass) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_weaponOption = weapons;
        m_astralCompass = astralCompass;

        // Add the Prime
        auto primeModel = new Model(BASESIZE, wounds());
        if (m_weaponOption == StormSabre) {
            primeModel->addMeleeWeapon(&m_stormSabrePrime);
        } else if (m_weaponOption == ShockHandaxe) {
            primeModel->addMeleeWeapon(&m_shockHandaxePrime);
        }
        primeModel->addMissileWeapon(&m_boltstormPistolPrime);
        addModel(primeModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (m_weaponOption == StormSabre) {
                model->addMeleeWeapon(&m_stormSabre);
            } else if (m_weaponOption == ShockHandaxe) {
                model->addMeleeWeapon(&m_shockHandaxe);
            }
            model->addMissileWeapon(&m_shockHandaxePrime);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *VanguardHunters::Create(const ParameterList &parameters) {
        auto hunters = new VanguardHunters();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, StormSabre);
        bool astralCompass = GetBoolParam("Astral Compass", parameters, false);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, StormcastEternal::None);
        hunters->setStormhost(stormhost);

        bool ok = hunters->configure(numModels, weapons, astralCompass);
        if (!ok) {
            delete hunters;
            hunters = nullptr;
        }
        return hunters;
    }

    void VanguardHunters::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Weapons", StormSabre, ShockHandaxe, StormSabre, 1},
                            {ParamType::Boolean, "Astral Compass", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None,
                             StormcastEternal::AstralTemplars, 1},
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Vanguard-Hunters", factoryMethod);
        }
    }

    std::string VanguardHunters::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == StormSabre) {
                return "Storm Sabre";
            } else if (parameter.intValue == ShockHandaxe) {
                return "Shock Handaxe";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int VanguardHunters::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Storm Sabre") {
            return StormSabre;
        } else if (enumString == "Shock Handaxe") {
            return ShockHandaxe;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int VanguardHunters::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace StormcastEternals
