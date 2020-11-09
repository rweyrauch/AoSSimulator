/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Retributors.h>
#include <UnitFactory.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Retributors::s_registered = false;

    Retributors::Retributors() :
            StormcastEternal("Retributors", 4, g_wounds, 7, 4, false),
            m_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 2, 3, 3, -1, 2),
            m_lightningHammerPrime(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2),
            m_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 0, 0, 0, 0, 0) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, RETRIBUTORS};
        m_weapons = {&m_lightningHammer, &m_lightningHammerPrime, &m_starsoulMace};
    }

    bool Retributors::configure(int numModels, int numStarsoulMaces) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }
        const int maxStarsoulMaces = (numModels / 5) * 2;
        if (numStarsoulMaces > maxStarsoulMaces) {
            // Invalid weapon configuration.
            return false;
        }

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        primeModel->addMeleeWeapon(&m_lightningHammerPrime);
        addModel(primeModel);

        for (auto i = 0; i < numStarsoulMaces; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_starsoulMace);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_lightningHammer);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds Retributors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Blast to Ashes
        if ((hitRoll == 6) && (weapon->name() == m_lightningHammer.name())) {
            return {0, 2};
        }

        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Unit *Retributors::Create(const ParameterList &parameters) {
        auto unit = new Retributors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numStarsoulMaces = GetIntParam("Starsoul Maces", parameters, 0);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels, numStarsoulMaces);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Retributors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Starsoul Maces", 2, 0, (g_maxUnitSize / 5) * 2, 1),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Retributors", factoryMethod);
        }
    }

    void Retributors::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (m_meleeTarget) {
            for (const auto &ip : m_models) {
                // Starsoul Mace
                if (ip->preferredWeapon()->name() == m_starsoulMace.name()) {
                    int mortalWounds = 0;
                    int roll = Dice::RollD6();
                    if (roll >= 6) {
                        mortalWounds = Dice::RollD3() + 1;
                    } else if (roll >= 2) {
                        mortalWounds = Dice::RollD3();
                    }

                    m_meleeTarget->applyDamage({0, mortalWounds});
                }
            }
        }
    }

    int Retributors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
