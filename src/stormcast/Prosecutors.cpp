/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Prosecutors.h>
#include <UnitFactory.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Prosecutors::s_registered = false;

    Prosecutors::Prosecutors(Stormhost stormhost, int numModels, WeaponOption weapons, GrandWeaponOption primeGrandWeapon,
                             int numTridents, int numGrandaxes, int numGrandblades, int numGrandhammers) :
            StormcastEternal(stormhost, "Prosecutors", 12, g_wounds, 7, 4, true),
            m_celestialHammersMissile(Weapon::Type::Missile, "Celestial Hammer(s)", 18, 2, 4, 4, 0, 1),
            m_stormcallJavelinMissile(Weapon::Type::Missile, "Stormcall Javelin", 18, 1, 3, 3, 0, 1),
            m_stormcallJavelinMissilePrime(Weapon::Type::Missile, "Stormcall Javelin", 18, 2, 3, 3, 0, 1),
            m_stormsurgeTridentMissile(Weapon::Type::Missile, "Stormsurge Trident", 18, 1, 3, 3, -1, 2),
            m_stormsurgeTridentMissilePrime(Weapon::Type::Missile, "Stormsurge Trident", 18, 2, 3, 3, -1, 2),
            m_celestialHammers(Weapon::Type::Melee, "Celestial Hammers(s)", 1, 2, 3, 3, 0, 1),
            m_celestialHammersPrime(Weapon::Type::Melee, "Celestial Hammers(s)", 1, 3, 3, 3, 0, 1),
            m_grandaxe(Weapon::Type::Melee, "Grandaxe", 1, 0, 3, 3, -1, 1),
            m_grandblade(Weapon::Type::Melee, "Grandblade", 1, 2, 3, 4, -1, 2),
            m_grandhammer(Weapon::Type::Melee, "Grandhammer", 1, 2, 4, 3, -1, 2),
            m_stormcallJavelin(Weapon::Type::Melee, "Stormcall Javelin", 2, 1, 4, 4, 0, 1),
            m_stormsurgeTrident(Weapon::Type::Melee, "Stormsurge Trident", 2, 1, 4, 4, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, ANGELOS, PROSECUTORS};
        m_weapons = {&m_celestialHammersMissile,
                     &m_stormcallJavelinMissile,
                     &m_stormcallJavelinMissilePrime,
                     &m_stormsurgeTridentMissile,
                     &m_stormsurgeTridentMissilePrime,
                     &m_celestialHammers,
                     &m_celestialHammersPrime,
                     &m_grandaxe,
                     &m_grandblade,
                     &m_grandhammer,
                     &m_stormcallJavelin,
                     &m_stormsurgeTrident};

        // Heralds of Righteousness
        m_maxChargeDistance = 18.0;

        m_weaponOption = weapons;

        auto prime = new Model(g_basesize, wounds());
        switch (primeGrandWeapon) {
            case No_Grand_Weapon:
                if (weapons == Celestial_Hammer_And_Shield || weapons == Paired_Celestial_Hammers) {
                    prime->addMissileWeapon(&m_celestialHammersMissile);
                    prime->addMeleeWeapon(&m_celestialHammersPrime);
                } else if (weapons == Stormcall_Javelin_And_Shield) {
                    prime->addMissileWeapon(&m_stormcallJavelinMissilePrime);
                    prime->addMeleeWeapon(&m_stormcallJavelin);
                }
                break;
            case Stormsurge_Trident:
                prime->addMissileWeapon(&m_stormsurgeTridentMissilePrime);
                prime->addMeleeWeapon(&m_stormsurgeTrident);
                numTridents--;
                break;
            case Grandaxe:
                prime->addMeleeWeapon(&m_grandaxe);
                numGrandaxes--;
                break;
            case Grandblade:
                prime->addMeleeWeapon(&m_grandblade);
                numGrandblades--;
                break;
            case Grandhammer:
                prime->addMeleeWeapon(&m_grandhammer);
                numGrandhammers--;
                break;
            default:
                PLOG_INFO.printf("Unknown Prosecutor prime weapon %d", primeGrandWeapon);
                break;
        }
        addModel(prime);

        for (auto i = 0; i < numTridents; i++) {
            auto tridentModel = new Model(g_basesize, wounds());
            tridentModel->addMissileWeapon(&m_stormsurgeTridentMissile);
            tridentModel->addMeleeWeapon(&m_stormsurgeTrident);
            addModel(tridentModel);
        }
        for (auto i = 0; i < numGrandaxes; i++) {
            auto grandaxeModel = new Model(g_basesize, wounds());
            grandaxeModel->addMeleeWeapon(&m_grandaxe);
            addModel(grandaxeModel);
        }
        for (auto i = 0; i < numGrandblades; i++) {
            auto grandbladeModel = new Model(g_basesize, wounds());
            grandbladeModel->addMeleeWeapon(&m_grandblade);
            addModel(grandbladeModel);
        }
        for (auto i = 0; i < numGrandhammers; i++) {
            auto grandhammerModel = new Model(g_basesize, wounds());
            grandhammerModel->addMeleeWeapon(&m_grandhammer);
            addModel(grandhammerModel);
        }
        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Celestial_Hammer_And_Shield || m_weaponOption == Paired_Celestial_Hammers) {
                model->addMissileWeapon(&m_celestialHammersMissile);
                model->addMeleeWeapon(&m_celestialHammers);
            } else if (m_weaponOption == Stormcall_Javelin_And_Shield) {
                model->addMissileWeapon(&m_stormcallJavelinMissile);
                model->addMeleeWeapon(&m_stormcallJavelin);
            } else {
                PLOG_INFO.printf("Unknown Prosecutor weapon %d", m_weaponOption);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Wounds Prosecutors::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Stormcall Javelin
        if (weapon->name() == m_stormcallJavelin.name()) {
            if (distanceTo(target) >= 9.0) {
                return {weapon->damage() + 1, 0};
            }
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Unit *Prosecutors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Celestial_Hammer_And_Shield);
        GrandWeaponOption primeGrandWeapon = (GrandWeaponOption) GetEnumParam("Prime Grand Weapon", parameters,No_Grand_Weapon);
        int numTridents = GetIntParam("Stormsurge Tridents", parameters, 0);
        int numGrandaxes = GetIntParam("Grandaxes", parameters, 0);
        int numGrandhammers = GetIntParam("Grandhammers", parameters, 0);
        int numGrandblades = GetIntParam("Grandblades", parameters, 0);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);

        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return nullptr;
        }

        int maxGrandWeapons = numModels / 3;
        int totalGrandWeapons = numGrandaxes + numGrandblades + numGrandhammers;

        int maxTridents = 0;
        int totalTridents = numTridents;

        if (primeGrandWeapon == Stormsurge_Trident) {
            totalTridents++;
        }
        if (primeGrandWeapon == Grandaxe || primeGrandWeapon == Grandblade || primeGrandWeapon == Grandhammer) {
            totalGrandWeapons++;
        }

        // 1 in 3 can have a trident when using javelins
        if (weapons == Stormcall_Javelin_And_Shield) {
            maxTridents = numModels / 3;
            maxGrandWeapons = 0;
        }

        if (totalTridents > maxTridents) {
            return nullptr;
        }
        if (totalGrandWeapons > maxGrandWeapons) {
            return nullptr;
        }

        return new Prosecutors(stormhost, numModels, weapons, primeGrandWeapon, numTridents, numGrandaxes, numGrandblades,
                                  numGrandhammers);
    }

    std::string Prosecutors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Stormcall_Javelin_And_Shield) {
                return "Stormcall Javelin and Shield";
            } else if (parameter.intValue == Celestial_Hammer_And_Shield) {
                return "Celestial Hammer and Shield";
            } else if (parameter.intValue == Paired_Celestial_Hammers) {
                return "Paired Celestial Hammers";
            }
        } else if (std::string(parameter.name) == "Prime Grand Weapon") {
            if (parameter.intValue == No_Grand_Weapon) {
                return "No Grand Weapon";
            } else if (parameter.intValue == Stormsurge_Trident) {
                return "Stormsurge Trident";
            } else if (parameter.intValue == Grandaxe) {
                return "Grandaxe";
            } else if (parameter.intValue == Grandblade) {
                return "Grandblade";
            } else if (parameter.intValue == Grandhammer) {
                return "Grandhammer";
            }
        }

        return StormcastEternal::ValueToString(parameter);
    }

    int Prosecutors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Stormcall Javelin and Shield") {
            return Stormcall_Javelin_And_Shield;
        } else if (enumString == "Celestial Hammer and Shield") {
            return Celestial_Hammer_And_Shield;
        } else if (enumString == "Paired Celestial Hammers") {
            return Paired_Celestial_Hammers;
        } else if (enumString == "No Grand Weapon") {
            return No_Grand_Weapon;
        } else if (enumString == "Stormsurge Trident") {
            return Stormsurge_Trident;
        } else if (enumString == "Grandaxe") {
            return Grandaxe;
        } else if (enumString == "Grandblade") {
            return Grandblade;
        } else if (enumString == "Grandhammer") {
            return Grandhammer;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    void Prosecutors::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Stormcall_Javelin_And_Shield,
                                                       Paired_Celestial_Hammers, Celestial_Hammer_And_Shield};
            static const std::array<int, 5> primeWeapons = {No_Grand_Weapon,
                                                            Stormsurge_Trident, Grandaxe, Grandblade, Grandhammer};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Stormcall_Javelin_And_Shield, weapons),
                            EnumParameter("Prime Grand Weapon", No_Grand_Weapon, primeWeapons),
                            IntegerParameter("Stormsurge Tridents", 0, 0, g_maxUnitSize / 3, 1),
                            IntegerParameter("Grandaxes", 0, 0, g_maxUnitSize / 3, 1),
                            IntegerParameter("Grandblades", 0, 0, g_maxUnitSize / 3, 1),
                            IntegerParameter("Grandhammers", 0, 0, g_maxUnitSize / 3, 1),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };
            s_registered = UnitFactory::Register("Prosecutors", factoryMethod);
        }
    }

    Rerolls Prosecutors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if ((m_weaponOption == Paired_Celestial_Hammers) && (weapon->name() == m_celestialHammers.name())) {
            return Rerolls::Ones;
        }
        return StormcastEternal::toHitRerolls(weapon, target);
    }

    Rerolls Prosecutors::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Sigmarite Shields
        if (m_weaponOption == Celestial_Hammer_And_Shield || m_weaponOption == Stormcall_Javelin_And_Shield) {
            return Rerolls::Ones;
        }
        return StormcastEternal::toSaveRerolls(weapon, attacker);
    }

    int Prosecutors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = StormcastEternal::extraAttacks(nullptr, weapon, target);
        if (weapon->name() == m_grandaxe.name()) {
            // count the number of models in the unit w/in 1 in of this model
            extra += target->numModelsWithin(attackingModel, (double) weapon->range());
        }
        return extra;
    }

    int Prosecutors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Prosecutors::rollChargeDistance() {
        // Heralds of Righteousness
        m_unmodifiedChargeRoll = Dice::Roll3D6();
        if (!m_movementRules[MovementRule::Halve_Charge_Roll].empty()) {
            if (m_movementRules[MovementRule::Halve_Charge_Roll].front().allowed) {
                m_unmodifiedChargeRoll = (m_unmodifiedChargeRoll + 1) / 2; // Round up
            }
        }
        return m_unmodifiedChargeRoll + chargeModifier();
    }

} //namespace StormcastEternals
