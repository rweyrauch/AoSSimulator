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
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Judicators::s_registered = false;

    Judicators::Judicators() :
            StormcastEternal("Judicators", 5, g_wounds, 7, 4, false),
            m_skyboltBow(Weapon::Type::Missile, "Skybolt Bow", 24, 1, 3, 3, -1, 1),
            m_skyboltPrime(Weapon::Type::Missile, "Skybolt Bow", 24, 1, 2, 3, -1, 1),
            m_boltstormCrossbow(Weapon::Type::Missile, "Boltstorm Crossbow", 12, 3, 3, 4, 0, 1),
            m_shockboltBow(Weapon::Type::Missile, "Shockbolt Bow", 24, 1, 3, 3, -1, 1),
            m_shockboltPrime(Weapon::Type::Missile, "Shockbolt Bow", 24, 1, 2, 3, -1, 1),
            m_thunderboldCrossbow(Weapon::Type::Missile, "Thunderbolt Crossbow", 18, 0, 0, 0, 0, 0),
            m_stormGladius(Weapon::Type::Melee, "Storm Gladius", 1, 1, 3, 4, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, JUDICATORS};
        m_weapons = {&m_skyboltBow, &m_skyboltPrime, &m_boltstormCrossbow, &m_shockboltBow, &m_shockboltPrime,
                     &m_thunderboldCrossbow, &m_stormGladius};

        // Chained Lightning
        m_shockboltBow.setHitsPerAttack(RAND_D6);
    }

    bool Judicators::configure(int numModels, WeaponOption weapons, int numShockboltBows, int numThunderboltCrossbows) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }
        const int maxSpecialWeapons = numModels / 5;
        if (numShockboltBows + numThunderboltCrossbows > maxSpecialWeapons) {
            // Invalid weapon configuration.
            return false;
        }

        m_weaponOption = weapons;

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        if (m_weaponOption == Skybolt_Bow) {
            primeModel->addMissileWeapon(&m_skyboltPrime);
        } else if (m_weaponOption == Boltstorm_Crossbow) {
            primeModel->addMissileWeapon(&m_shockboltPrime);
        }
        primeModel->addMeleeWeapon(&m_stormGladius);
        addModel(primeModel);

        for (auto i = 0; i < numShockboltBows; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_shockboltBow);
            model->addMeleeWeapon(&m_stormGladius);
            addModel(model);
        }
        for (auto i = 0; i < numThunderboltCrossbows; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_thunderboldCrossbow);
            model->addMeleeWeapon(&m_stormGladius);
            addModel(model);
        }
        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Skybolt_Bow) {
                model->addMissileWeapon(&m_skyboltBow);
            } else if (m_weaponOption == Boltstorm_Crossbow) {
                model->addMissileWeapon(&m_boltstormCrossbow);
            }
            model->addMeleeWeapon(&m_stormGladius);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Rerolls Judicators::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // External Judgement
        if (unit->hasKeyword(CHAOS) && weapon->isMissile()) {
            return Reroll_Ones;
        }
        return StormcastEternal::toHitRerolls(weapon, unit);
    }

    int Judicators::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = StormcastEternal::extraAttacks(attackingModel, weapon, target);
        // Rapid Fire
        if (!m_ran && (weapon->name() == m_boltstormCrossbow.name())) {
            extra++;
        }
        return extra;
    }

    Unit *Judicators::Create(const ParameterList &parameters) {
        auto juds = new Judicators();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Skybolt_Bow);
        int numShockboltBows = GetIntParam("Shockbolt Bows", parameters, 0);
        int numThunderboltCrossbows = GetIntParam("Thunderbolt Crossbows", parameters, 0);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        juds->setStormhost(stormhost);

        bool ok = juds->configure(numModels, weapons, numShockboltBows, numThunderboltCrossbows);
        if (!ok) {
            delete juds;
            juds = nullptr;
        }
        return juds;
    }

    void Judicators::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Skybolt_Bow, Boltstorm_Crossbow};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Skybolt_Bow, weapons),
                            IntegerParameter("Shockbolt Bows", 1, 0, g_maxUnitSize / 5, 1),
                            IntegerParameter("Thunderbolt Crossbows", 0, 0, g_maxUnitSize / 5, 1),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Judicators", factoryMethod);
        }
    }

    std::string Judicators::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Skybolt_Bow) {
                return "Skybolt Bow";
            } else if (parameter.intValue == Boltstorm_Crossbow) {
                return "Boltstorm Crossbow";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int Judicators::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Skybolt Bow") {
            return Skybolt_Bow;
        } else if (enumString == "Boltstorm Crossbow") {
            return Boltstorm_Crossbow;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int Judicators::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Chained Lightning
        if (weapon->name() == m_shockboltBow.name()) {
            return Dice::RollD6();
        }
        return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    void Judicators::onStartShooting(PlayerId player) {
        StormcastEternal::onStartShooting(player);

        if (m_shootingTarget) {
            for (const auto &ip : m_models) {
                // Thunderbolt Crossbow
                if (ip->hasWeapon(m_thunderboldCrossbow.name())) {
                    int roll = Dice::RollD6();
                    if (m_shootingTarget->hasKeyword(MONSTER))
                        roll--;

                    if (roll <= m_shootingTarget->remainingModels()) {
                        m_shootingTarget->applyDamage({0, Dice::RollD3()}, this);
                    }
                }
            }
        }

    }

    int Judicators::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
