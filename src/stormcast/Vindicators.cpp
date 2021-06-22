/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/Vindicators.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 4 * g_pointsPerBlock;

    bool Vindicators::s_registered = false;

    Vindicators::Vindicators(Stormhost stormhost, int numModels, int points) :
            StormcastEternal(stormhost, "Vindicators", 5, g_wounds, 7, 3, false, points),
            m_spear(Weapon::Type::Melee, "Stormspear", 2, 2, 3, 3, -1, 1),
            m_spearPrime(Weapon::Type::Melee, "Stormspear", 2, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, STORMCAST_ETERNAL, THUNDERSTRIKE, REDEEMER, VINDICTORS};
        m_weapons = {&m_spear, &m_spearPrime};
        m_battleFieldRole = Role::Battleline;

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        primeModel->addMeleeWeapon(&m_spearPrime);
        addModel(primeModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spear);
            addModel(model);
        }
    }

    Unit *Vindicators::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);

        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return nullptr;
        }

        return new Vindicators(stormhost, numModels, ComputePoints(parameters));
    }

    void Vindicators::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Vindicators", factoryMethod);
        }
    }

    int Vindicators::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
