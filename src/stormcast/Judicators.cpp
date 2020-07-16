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
    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    bool Judicators::s_registered = false;

    Judicators::Judicators() :
            StormcastEternal("Judicators", 5, WOUNDS, 7, 4, false),
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
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
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
        auto primeModel = new Model(BASESIZE, wounds());
        if (m_weaponOption == SkyboltBow) {
            primeModel->addMissileWeapon(&m_skyboltPrime);
        } else if (m_weaponOption == BoltstormCrossbow) {
            primeModel->addMissileWeapon(&m_shockboltPrime);
        }
        primeModel->addMeleeWeapon(&m_stormGladius);
        addModel(primeModel);

        for (auto i = 0; i < numShockboltBows; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_shockboltBow);
            model->addMeleeWeapon(&m_stormGladius);
            addModel(model);
        }
        for (auto i = 0; i < numThunderboltCrossbows; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_thunderboldCrossbow);
            model->addMeleeWeapon(&m_stormGladius);
            addModel(model);
        }
        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (m_weaponOption == SkyboltBow) {
                model->addMissileWeapon(&m_skyboltBow);
            } else if (m_weaponOption == BoltstormCrossbow) {
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
            return RerollOnes;
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
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, SkyboltBow);
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
            static const std::array<int, 2> weapons = {SkyboltBow, BoltstormCrossbow};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", SkyboltBow, weapons),
                            IntegerParameter("Shockbolt Bows", 1, 0, MAX_UNIT_SIZE / 5, 1),
                            IntegerParameter("Thunderbolt Crossbows", 0, 0, MAX_UNIT_SIZE / 5, 1),
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
            if (parameter.intValue == SkyboltBow) {
                return "Skybolt Bow";
            } else if (parameter.intValue == BoltstormCrossbow) {
                return "Boltstorm Crossbow";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int Judicators::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Skybolt Bow") {
            return SkyboltBow;
        } else if (enumString == "Boltstorm Crossbow") {
            return BoltstormCrossbow;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int Judicators::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Chained Lightning
        if (weapon->name() == m_shockboltBow.name()) {
            return Dice::rollD6();
        }
        return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    void Judicators::onStartShooting(PlayerId player) {
        StormcastEternal::onStartShooting(player);

        if (m_shootingTarget) {
            for (const auto &ip : m_models) {
                // Thunderbolt Crossbow
                if (ip->hasWeapon(m_thunderboldCrossbow.name())) {
                    int roll = Dice::rollD6();
                    if (m_shootingTarget->hasKeyword(MONSTER))
                        roll--;

                    if (roll <= m_shootingTarget->remainingModels()) {
                        m_shootingTarget->applyDamage({0, Dice::rollD3()});
                    }
                }
            }
        }

    }

    int Judicators::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace StormcastEternals
