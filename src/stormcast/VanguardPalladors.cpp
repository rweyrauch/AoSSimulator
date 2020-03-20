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

namespace StormcastEternals {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 720;

    bool VanguardPalladors::s_registered = false;

    VanguardPalladors::VanguardPalladors() :
            StormcastEternal("Vanguard-Palladors", 12, WOUNDS, 7, 4, false),
            m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 4, 0, 1),
            m_starstrikeJavelinMissile(Weapon::Type::Missile, "Starstrike Javelin", 18, 1, 3, 3, -1, 1),
            m_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 3, 3, 0, 1),
            m_starstrikeJavelin(Weapon::Type::Melee, "Starstrike Javelin", 2, 1, 4, 3, -1, 1),
            m_beakAndClaws(Weapon::Type::Melee, "Razor Beak and Claws", 1, 3, 3, 3, -2, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, GRYPH_CHARGER, STORMCAST_ETERNAL, ANGELOS, VANGUARD_PALLADORS};
        m_weapons = {&m_boltstormPistol, &m_starstrikeJavelinMissile, &m_shockHandaxe, &m_starstrikeJavelin,
                     &m_beakAndClaws};
    }

    bool VanguardPalladors::configure(int numModels, WeaponOption weapons) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        // Add the Prime
        auto primeModel = new Model(BASESIZE, wounds());
        if (weapons == ShockHandaxe) {
            primeModel->addMeleeWeapon(&m_shockHandaxe);
        } else {
            primeModel->addMissileWeapon(&m_starstrikeJavelinMissile);
            primeModel->addMeleeWeapon(&m_starstrikeJavelin);
        }
        primeModel->addMissileWeapon(&m_boltstormPistol);
        primeModel->addMeleeWeapon(&m_beakAndClaws);
        addModel(primeModel);

        auto currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == ShockHandaxe) {
                model->addMeleeWeapon(&m_shockHandaxe);
            } else {
                model->addMissileWeapon(&m_starstrikeJavelinMissile);
                model->addMeleeWeapon(&m_starstrikeJavelin);
            }
            model->addMissileWeapon(&m_boltstormPistol);
            model->addMeleeWeapon(&m_beakAndClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *VanguardPalladors::Create(const ParameterList &parameters) {
        auto *unit = new VanguardPalladors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, StarstrikeJavelin);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, StormcastEternal::None);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void VanguardPalladors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Weapons", StarstrikeJavelin, ShockHandaxe, StarstrikeJavelin, 1},
                            {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None,
                             StormcastEternal::AstralTemplars, 1},
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Vanguard-Palladors", factoryMethod);
        }
    }

    std::string VanguardPalladors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == StarstrikeJavelin) {
                return "Starstrike Javelin";
            } else if (parameter.intValue == ShockHandaxe) {
                return "Shock Handaxe";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int VanguardPalladors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Starstrike Javelin") {
            return StarstrikeJavelin;
        } else if (enumString == "Shock Handaxe") {
            return ShockHandaxe;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    Wounds VanguardPalladors::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        // TODO: make sure prime model exists
        // Lunar Blade
        if (distanceTo(m_meleeTarget) <= 1.0f) {
            int roll = Dice::rollD6();
            if (roll >= 2) {
                Wounds bladeWounds = {0, 1};
                m_meleeTarget->applyDamage(bladeWounds);
                wounds += bladeWounds;
            }
        }
        return wounds;
    }

    Wounds VanguardPalladors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Aethereal Strike
        if ((hitRoll == 6) && (weapon->name() == m_beakAndClaws.name())) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int VanguardPalladors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace StormcastEternals
