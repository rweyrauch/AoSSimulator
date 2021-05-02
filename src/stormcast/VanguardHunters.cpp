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
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 300;

    bool VanguardHunters::s_registered = false;

    VanguardHunters::VanguardHunters(Stormhost stormhost, int numModels, WeaponOption weapons, bool astralCompass, int points) :
            StormcastEternal(stormhost, "Vanguard-Hunters", 6, g_wounds, 7, 4, false, points),
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

        m_weaponOption = weapons;
        m_astralCompass = astralCompass;

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        if (m_weaponOption == Storm_Sabre) {
            primeModel->addMeleeWeapon(&m_stormSabrePrime);
        } else if (m_weaponOption == Shock_Handaxe) {
            primeModel->addMeleeWeapon(&m_shockHandaxePrime);
        }
        primeModel->addMissileWeapon(&m_boltstormPistolPrime);
        addModel(primeModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Storm_Sabre) {
                model->addMeleeWeapon(&m_stormSabre);
            } else if (m_weaponOption == Shock_Handaxe) {
                model->addMeleeWeapon(&m_shockHandaxe);
            }
            model->addMissileWeapon(&m_shockHandaxePrime);
            addModel(model);
        }
    }

    Unit *VanguardHunters::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Storm_Sabre);
        bool astralCompass = GetBoolParam("Astral Compass", parameters, false);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        return new VanguardHunters(stormhost, numModels, weapons, astralCompass, ComputePoints(parameters));
    }

    void VanguardHunters::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Shock_Handaxe, Storm_Sabre};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Storm_Sabre, weapons),
                            BoolParameter("Astral Compass"),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Vanguard-Hunters", factoryMethod);
        }
    }

    std::string VanguardHunters::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Storm_Sabre) {
                return "Storm Sabre";
            } else if (parameter.intValue == Shock_Handaxe) {
                return "Shock Handaxe";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int VanguardHunters::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Storm Sabre") {
            return Storm_Sabre;
        } else if (enumString == "Shock Handaxe") {
            return Shock_Handaxe;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int VanguardHunters::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
