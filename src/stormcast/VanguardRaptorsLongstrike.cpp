/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/VanguardRaptorsLongstrike.h>
#include <UnitFactory.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 170;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool VanguardRaptorsLongstrike::s_registered = false;

    VanguardRaptorsLongstrike::VanguardRaptorsLongstrike(Stormhost stormhost, int numModels) :
            StormcastEternal(stormhost, "Vanguard Raptors with Longstrike Crossbows", 5, g_wounds, 7, 4, false),
            m_longstikeCrossbow(Weapon::Type::Missile, "Longstrike Crossbow", 24, 1, 2, 3, -2, 2),
            m_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 1, 4, 3, 0, 1),
            m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 4, 3, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, VANGUARD_RAPTORS};
        m_weapons = {&m_longstikeCrossbow, &m_heavyStock, &m_beakAndClaws};

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        primeModel->addMissileWeapon(&m_longstikeCrossbow);
        primeModel->addMeleeWeapon(&m_heavyStock);
        primeModel->addMeleeWeapon(&m_beakAndClaws);
        addModel(primeModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_longstikeCrossbow);
            model->addMeleeWeapon(&m_heavyStock);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    void VanguardRaptorsLongstrike::onStartShooting(PlayerId player) {
        if (player == owningPlayer()) {
            // Longshot
            if (!m_moved) {
                m_longstikeCrossbow.setRange(m_longstikeCrossbow.range() + 6);
            } else {
                m_longstikeCrossbow.setRange(m_longstikeCrossbow.range());
            }
        }
        StormcastEternal::onStartShooting(player);
    }

    Unit *VanguardRaptorsLongstrike::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        return new VanguardRaptorsLongstrike(stormhost, numModels);
    }

    void VanguardRaptorsLongstrike::Init() {
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

            s_registered = UnitFactory::Register("Vanguard Raptors with Longstrike Crossbows", factoryMethod);
        }
    }

    Wounds VanguardRaptorsLongstrike::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                   int woundRoll) const {
        // Headshot
        if ((hitRoll == 6) && (weapon->name() == m_longstikeCrossbow.name())) {
            return {0, 2};
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int VanguardRaptorsLongstrike::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
