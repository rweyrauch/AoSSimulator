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

    ChaosSpawn::ChaosSpawn(Greatfray fray, int numModels, int points) :
            BeastsOfChaosBase("Chaos Spawn", RAND_2D6, g_wounds, 10, 5, false, points) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MORTAL, SLAVES_TO_DARKNESS, CHAOS_SPAWN};
        m_weapons = {&m_freakingMutations};

        setGreatfray(fray);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_freakingMutations);
            addModel(model);
        }
    }

    Unit *ChaosSpawn::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);

        return new ChaosSpawn(fray, numModels, ComputePoints(parameters));
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

    int ChaosSpawn::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void ChaosSpawn::onFriendlyUnitSlain(const Unit *attacker) {
        BeastsOfChaosBase::onFriendlyUnitSlain(nullptr);

        if (hasKeyword(KHORNE)) {
            // Add a Blood Tithe point
            getRoster()->incrementResource(1);
        }
    }

    void ChaosSpawn::onEnemyUnitSlain(const Unit *unit) {
        BeastsOfChaosBase::onEnemyUnitSlain(unit);

        if (hasKeyword(KHORNE)) {
            // Add a Blood Tithe point
            getRoster()->incrementResource(1);
        }
    }

} // namespace BeastsOfChaos