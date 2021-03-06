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
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 170;
    static const int g_pointsMaxUnitSize = 4 * g_pointsPerBlock;

    bool VanguardPalladors::s_registered = false;

    VanguardPalladors::VanguardPalladors(Stormhost stormhost, int numModels, WeaponOption weapons, int points) :
            StormcastEternal(stormhost, "Vanguard-Palladors", 12, g_wounds, 7, 4, false, points),
            m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 4, 0, 1),
            m_starstrikeJavelinMissile(Weapon::Type::Missile, "Starstrike Javelin", 18, 1, 3, 3, -1, 1),
            m_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 3, 3, 0, 1),
            m_starstrikeJavelin(Weapon::Type::Melee, "Starstrike Javelin", 2, 1, 4, 3, -1, 1),
            m_beakAndClaws(Weapon::Type::Melee, "Razor Beak and Claws", 1, 3, 3, 3, -2, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, GRYPH_CHARGER, STORMCAST_ETERNAL, ANGELOS, VANGUARD_PALLADORS};
        m_weapons = {&m_boltstormPistol, &m_starstrikeJavelinMissile, &m_shockHandaxe, &m_starstrikeJavelin,
                     &m_beakAndClaws};
        m_hasMount = true;
        m_beakAndClaws.setMount(true);

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        if (weapons == Shock_Handaxe) {
            primeModel->addMeleeWeapon(&m_shockHandaxe);
        } else {
            primeModel->addMissileWeapon(&m_starstrikeJavelinMissile);
            primeModel->addMeleeWeapon(&m_starstrikeJavelin);
        }
        primeModel->addMissileWeapon(&m_boltstormPistol);
        primeModel->addMeleeWeapon(&m_beakAndClaws);
        primeModel->setName("Prime");
        addModel(primeModel);

        auto currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Shock_Handaxe) {
                model->addMeleeWeapon(&m_shockHandaxe);
            } else {
                model->addMissileWeapon(&m_starstrikeJavelinMissile);
                model->addMeleeWeapon(&m_starstrikeJavelin);
            }
            model->addMissileWeapon(&m_boltstormPistol);
            model->addMeleeWeapon(&m_beakAndClaws);
            addModel(model);
        }
    }

    Unit *VanguardPalladors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Starstrike_Javelin);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        return new VanguardPalladors(stormhost, numModels, weapons, ComputePoints(parameters));
    }

    void VanguardPalladors::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Shock_Handaxe, Starstrike_Javelin};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Starstrike_Javelin, weapons),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Vanguard-Palladors", factoryMethod);
        }
    }

    std::string VanguardPalladors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Starstrike_Javelin) {
                return "Starstrike Javelin";
            } else if (parameter.intValue == Shock_Handaxe) {
                return "Shock Handaxe";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int VanguardPalladors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Starstrike Javelin") {
            return Starstrike_Javelin;
        } else if (enumString == "Shock Handaxe") {
            return Shock_Handaxe;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    void VanguardPalladors::onEndCombat(PlayerId player) {
        StormcastEternal::onEndCombat(player);

        if (isNamedModelAlive("Prime")) {
            // Lunar Blade
            if (distanceTo(m_meleeTarget) <= 1.0) {
                int roll = Dice::RollD6();
                if (roll >= 2) {
                    Wounds bladeWounds = {0, 1};
                    m_meleeTarget->applyDamage(bladeWounds, this);
                }
            }
        }
    }

    Wounds VanguardPalladors::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Aethereal Strike
        if ((hitRoll == 6) && (weapon->name() == m_beakAndClaws.name())) {
            return {0, 1};
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int VanguardPalladors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
