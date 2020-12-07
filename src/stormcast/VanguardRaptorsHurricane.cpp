/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/VanguardRaptorsHurricane.h>
#include <UnitFactory.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool VanguardRaptorsHurricane::s_registered = false;

    VanguardRaptorsHurricane::VanguardRaptorsHurricane() :
            StormcastEternal("Vanguard Raptors with Hurricane Crossbows", 5, g_wounds, 7, 4, false),
            m_hurricaneCrossbow(Weapon::Type::Missile, "Hurricane Crossbow", 18, 6, 4, 4, 0, 1),
            m_hurricaneCrossbowPrime(Weapon::Type::Missile, "Hurricane Crossbow", 18, 6, 3, 4, 0, 1),
            m_heavyStock(Weapon::Type::Melee, "Heavy Stock", 1, 1, 4, 4, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, JUSTICAR, VANGUARD_RAPTORS};
        m_weapons = {&m_hurricaneCrossbow, &m_hurricaneCrossbowPrime, &m_heavyStock};
    }

    bool VanguardRaptorsHurricane::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        primeModel->addMissileWeapon(&m_hurricaneCrossbowPrime);
        primeModel->addMeleeWeapon(&m_heavyStock);
        addModel(primeModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_hurricaneCrossbow);
            model->addMeleeWeapon(&m_heavyStock);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *VanguardRaptorsHurricane::Create(const ParameterList &parameters) {
        auto unit = new VanguardRaptorsHurricane();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void VanguardRaptorsHurricane::Init() {
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

            s_registered = UnitFactory::Register("Vanguard Raptors with Hurricane Crossbows", factoryMethod);
        }
    }

    int VanguardRaptorsHurricane::extraAttacks(const Model *attackingModel, const Weapon *weapon,
                                               const Unit *target) const {
        // Rapid Fire
        if (!m_moved && weapon->name() == m_hurricaneCrossbow.name()) {
            return 3;
        }
        return StormcastEternal::extraAttacks(attackingModel, weapon, target);
    }

    int VanguardRaptorsHurricane::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
