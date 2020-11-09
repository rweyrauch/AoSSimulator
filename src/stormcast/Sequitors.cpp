/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Sequitors.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 480;

    bool Sequitors::s_registered = false;

    Sequitors::Sequitors() :
            StormcastEternal("Sequitors", 5, g_wounds, 7, 4, false),
            m_stormsmiteMaul(Weapon::Type::Melee, "Stormsmite Maul", 1, 2, 3, 3, 0, 1),
            m_stormsmiteMaulPrime(Weapon::Type::Melee, "Stormsmite Maul", 1, 3, 3, 3, 0, 1),
            m_tempestBlade(Weapon::Type::Melee, "Tempest Blade", 1, 3, 3, 4, 0, 1),
            m_tempestBladePrime(Weapon::Type::Melee, "Tempest Blade", 1, 4, 3, 4, 0, 1),
            m_stormsmiteGreatmace(Weapon::Type::Melee, "Stormsmite Greatmace", 1, 2, 3, 3, -1, 2),
            m_stormsmiteGreatmacePrime(Weapon::Type::Melee, "Stormsmite Greatmace", 1, 3, 3, 3, -1, 2),
            m_redemptionCache(Weapon::Type::Missile, "Redemption Cache", 6, 1, 4, 0, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, REDEEMER, SEQUITORS};
        m_weapons = {&m_stormsmiteMaul,
                     &m_stormsmiteMaulPrime,
                     &m_tempestBlade,
                     &m_tempestBladePrime,
                     &m_stormsmiteGreatmace,
                     &m_stormsmiteGreatmacePrime,
                     &m_redemptionCache};
    }

    bool Sequitors::configure(int numModels, WeaponOption weapons, int numGreatmaces, bool primeGreatmace,
                              bool redemptionCache) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }
        const int maxGrandmaces = (numModels / 5) * 2;
        if (numGreatmaces > maxGrandmaces) {
            // Invalid weapon configuration.
            return false;
        }
        if (primeGreatmace && redemptionCache) {
            // Invalid weapon configuration.
            return false;
        }

        m_weaponOption = weapons;
        m_haveRedemptionCache = redemptionCache;

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        if (primeGreatmace) {
            primeModel->addMeleeWeapon(&m_stormsmiteGreatmacePrime);
        } else if (m_weaponOption == Stormsmite_Maul) {
            primeModel->addMeleeWeapon(&m_stormsmiteMaulPrime);
        } else if (m_weaponOption == Tempest_Blade) {
            primeModel->addMeleeWeapon(&m_tempestBladePrime);
        }
        if (m_haveRedemptionCache) {
            primeModel->addMissileWeapon(&m_redemptionCache);
        }
        addModel(primeModel);

        for (auto i = 0; i < numGreatmaces; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_stormsmiteGreatmace);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Stormsmite_Maul) {
                model->addMeleeWeapon(&m_stormsmiteMaul);
            } else if (m_weaponOption == Tempest_Blade) {
                model->addMeleeWeapon(&m_tempestBlade);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Rerolls Sequitors::toSaveRerolls(const Weapon *weapon) const {
        const bool isMissile = weapon->isMissile();

        // Soulshields Shields
        for (const auto &m : m_models) {
            // check if remaining models have a shield
            for (auto ip = m->meleeWeaponBegin(); ip != m->meleeWeaponEnd(); ++ip) {
                if ((*ip)->name() == m_stormsmiteMaul.name() || (*ip)->name() == m_tempestBlade.name()) {
                    if (m_aethericChannellingWeapons || isMissile) {
                        return Reroll_Ones; // weapons empowered
                    } else {
                        return Reroll_Failed;
                    } // shields empowered
                }
            }
        }

        return StormcastEternal::toSaveRerolls(weapon);
    }

    Rerolls Sequitors::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Aetheric Channeling
        if (m_aethericChannellingWeapons) {
            return Reroll_Failed;
        }
        return StormcastEternal::toHitRerolls(weapon, unit);
    }

    int Sequitors::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Greatmace Blast
        if (unmodifiedHitRoll == 6) {
            if ((weapon->name() == m_stormsmiteGreatmace.name()) &&
                (unit->hasKeyword(DAEMON) || unit->hasKeyword(NIGHTHAUNT))) {
                // each 6 inflicts d3 hits instead of 1
                return Dice::RollD3();
            }
        }
        return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    Unit *Sequitors::Create(const ParameterList &parameters) {
        auto unit = new Sequitors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Stormsmite_Maul);
        int numGreatmaces = GetIntParam("Greatmaces", parameters, 0);
        bool primeGreatmace = GetBoolParam("Prime Greatmace", parameters, false);
        bool redemptionCache = GetBoolParam("Redemption Cache", parameters, false);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels, weapons, numGreatmaces, primeGreatmace, redemptionCache);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Sequitors::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Stormsmite_Maul, Tempest_Blade};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Stormsmite_Maul, weapons),
                            IntegerParameter("Greatmaces", 2, 0, g_maxUnitSize / 5 * 2, 1),
                            BoolParameter("Prime Greatmace"),
                            BoolParameter("Redemption Cache"),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Sequitors", factoryMethod);
        }
    }

    std::string Sequitors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Stormsmite_Maul) {
                return "Stormsmite Maul";
            } else if (parameter.intValue == Tempest_Blade) {
                return "Tempest Blade";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int Sequitors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Stormsmite Maul") {
            return Stormsmite_Maul;
        } else if (enumString == "Tempest Blade") {
            return Tempest_Blade;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    void Sequitors::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if ((player == owningPlayer()) && m_haveRedemptionCache) {
            // Redemption Cache
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);
            for (auto ip : units) {
                if (ip->hasKeyword(CHAOS) || ip->hasKeyword(DEATH)) {
                    int roll = Dice::RollD6();
                    if (roll >= 4) {
                        ip->applyDamage({0, 1});
                        break;
                    }
                }
            }
        }
    }

    int Sequitors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
