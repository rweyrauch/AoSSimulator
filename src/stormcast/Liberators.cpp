/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/Liberators.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 480;

    bool Liberators::s_registered = false;

    Liberators::Liberators() :
            StormcastEternal("Liberators", 5, g_wounds, 7, 4, false),
            m_warhammer(Weapon::Type::Melee, "Warhammer", 1, 2, 4, 3, 0, 1),
            m_warhammerPrime(Weapon::Type::Melee, "Warhammer", 1, 3, 4, 3, 0, 1),
            m_warblade(Weapon::Type::Melee, "Warblade", 1, 2, 3, 4, 0, 1),
            m_warbladePrime(Weapon::Type::Melee, "Warblade", 1, 3, 3, 4, 0, 1),
            m_grandhammer(Weapon::Type::Melee, "Grandhammer", 1, 2, 4, 3, -1, 2),
            m_grandblade(Weapon::Type::Melee, "Grandblade", 1, 2, 3, 4, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, REDEEMER, LIBERATORS};
        m_weapons = {&m_warhammer, &m_warhammerPrime, &m_warblade, &m_warbladePrime, &m_grandhammer, &m_grandblade};
        m_battleFieldRole = Battleline;
    }

    bool
    Liberators::configure(int numModels, WeaponOption weapons, bool pairedWeapons, int numGrandhammers,
                          int numGrandblades) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }
        const int maxGrandweapons = numModels / 5;
        if (numGrandblades + numGrandhammers > maxGrandweapons) {
            // Invalid weapon configuration.
            return false;
        }

        m_weaponOption = weapons;
        m_pairedWeapons = pairedWeapons;

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        if (m_weaponOption == Warhammer) {
            primeModel->addMeleeWeapon(&m_warhammerPrime);
        } else if (m_weaponOption == Warblade) {
            primeModel->addMeleeWeapon(&m_warbladePrime);
        }
        addModel(primeModel);

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
            if (m_weaponOption == Warhammer) {
                model->addMeleeWeapon(&m_warhammer);
            } else if (m_weaponOption == Warblade) {
                model->addMeleeWeapon(&m_warblade);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Liberators::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = StormcastEternal::toHitModifier(weapon, unit);

        // Lay Low the Tyrants
        if (unit->wounds() >= 5) {
            modifier += 1;
        }

        return modifier;
    }

    Rerolls Liberators::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Sigmarite Shields
        if (m_weaponOption == Warhammer || m_weaponOption == Warblade) {
            return Reroll_Ones;
        }

        return StormcastEternal::toSaveRerolls(weapon, attacker);
    }

    int Liberators::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        if (unmodifiedHitRoll == 6) {
            if (m_pairedWeapons && (weapon->name() == m_warblade.name() || weapon->name() == m_warhammer.name())) {
                // each 6 inflicts an additional hit
                return 2;
            }
        }
        return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    Unit *Liberators::Create(const ParameterList &parameters) {
        auto libs = new Liberators();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Warblade);
        bool pairedWeapons = GetBoolParam("Paired Weapons", parameters, false);
        int numGrandhammers = GetIntParam("Grandhammers", parameters, 0);
        int numGrandblades = GetIntParam("Grandblades", parameters, 0);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        libs->setStormhost(stormhost);

        bool ok = libs->configure(numModels, weapons, pairedWeapons, numGrandhammers, numGrandblades);
        if (!ok) {
            delete libs;
            libs = nullptr;
        }
        return libs;
    }

    void Liberators::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Warhammer, Warblade};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Warhammer, weapons),
                            BoolParameter( "Paired Weapons"),
                            IntegerParameter("Grandhammers", 0, 0, g_maxUnitSize / 5, 1),
                            IntegerParameter("Grandblades", 0, 0, g_maxUnitSize / 5, 1),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Liberators", factoryMethod);
        }
    }

    std::string Liberators::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Warhammer) {
                return "Warhammer";
            } else if (parameter.intValue == Warblade) {
                return "Warblade";
            }
        }

        return StormcastEternal::ValueToString(parameter);
    }

    int Liberators::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Warhammer") {
            return Warhammer;
        } else if (enumString == "Warblade") {
            return Warblade;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int Liberators::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
