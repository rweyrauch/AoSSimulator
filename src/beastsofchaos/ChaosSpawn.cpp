/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/ChaosSpawn.h>
#include <UnitFactory.h>
#include <Roster.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 50;
    static const int g_wounds = 5;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 50;
    static const int g_pointsMaxUnitSize = 300;

    bool ChaosSpawn::s_registered = false;

    ChaosSpawn::ChaosSpawn() :
            BeastsOfChaosBase("Chaos Spawn", RAND_2D6, g_wounds, 10, 5, false) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MORTAL, SLAVES_TO_DARKNESS, CHAOS_SPAWN};
        m_weapons = {&m_freakingMutations};
    }

    bool ChaosSpawn::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_freakingMutations);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *ChaosSpawn::Create(const ParameterList &parameters) {
        auto unit = new ChaosSpawn();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosSpawn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };

            s_registered = UnitFactory::Register("Chaos Spawn", factoryMethod);
        }
    }

    int ChaosSpawn::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void ChaosSpawn::onFriendlyUnitSlain(const Unit *attacker) {
        Unit::onFriendlyUnitSlain(nullptr);

        if (hasKeyword(KHORNE)) {
            // Add a Blood Tithe point
            getRoster()->incrementResource(1);
        }
    }

    void ChaosSpawn::onEnemyUnitSlain(const Unit *unit) {
        Unit::onEnemyUnitSlain(unit);

        if (hasKeyword(KHORNE)) {
            // Add a Blood Tithe point
            getRoster()->incrementResource(1);
        }
    }

} // namespace BeastsOfChaos