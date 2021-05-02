/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/Castigators.h>
#include <UnitFactory.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 18;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 420;

    bool Castigators::s_registered = false;

    Castigators::Castigators(Stormhost stormhost, int numModels, int points) :
            StormcastEternal(stormhost, "Castigators", 5, g_wounds, 7, 4, false, points),
            m_thunderheadGreatbow(Weapon::Type::Missile, "Thunderhead Greatbow", 18, 1, 3, 3, -1, 1),
            m_thunderheadGreatbowPrime(Weapon::Type::Missile, "Thunderhead Greatbow", 18, 1, 2, 3, -1, 1),
            m_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, JUSTICAR, CASTIGATORS};
        m_weapons = {&m_thunderheadGreatbow, &m_thunderheadGreatbowPrime, &m_heavyStock};

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        primeModel->addMissileWeapon(&m_thunderheadGreatbowPrime);
        primeModel->addMeleeWeapon(&m_heavyStock);
        addModel(primeModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_thunderheadGreatbow);
            model->addMeleeWeapon(&m_heavyStock);
            addModel(model);
        }

        //m_points = computePoints();
    }

    Unit *Castigators::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        return new Castigators(stormhost, numModels, ComputePoints(parameters));
    }

    void Castigators::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Castigators", factoryMethod);
        }
    }

    int Castigators::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Burst of Celestial Energy
        if (unmodifiedHitRoll == 6) {
            if (weapon->name() == m_thunderheadGreatbow.name()) {
                if (unit->hasKeyword(DAEMON) || unit->hasKeyword(NIGHTHAUNT)) {
                    return Dice::RollD3();
                }
            }
        }
        return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    void Castigators::onStartShooting(PlayerId player) {
        if (player == owningPlayer()) {
            // Aetheric Channelling
            if (m_shootingTarget) {
                if (m_shootingTarget->ignoreRend())
                    m_aethericChannellingPower = false;
                else
                    m_aethericChannellingPower = (m_shootingTarget->save() < 4);
            }
        }

        if (m_aethericChannellingPower) {
            m_thunderheadGreatbow.setRend(-2);
            m_thunderheadGreatbowPrime.setRend(-2);
        } else {
            m_thunderheadGreatbow.setRend(m_thunderheadGreatbow.rend());
            m_thunderheadGreatbowPrime.setRend(m_thunderheadGreatbowPrime.rend());
        }

        StormcastEternal::onStartShooting(player);
    }

    Rerolls Castigators::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        if (weapon->name() == m_thunderheadGreatbow.name()) {
            if (!m_aethericChannellingPower) {
                return Rerolls::Ones;
            }
        }
        return StormcastEternal::toHitRerolls(weapon, unit);
    }

    int Castigators::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
