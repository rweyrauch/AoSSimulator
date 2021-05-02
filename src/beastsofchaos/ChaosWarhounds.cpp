/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/ChaosWarhounds.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 210;

    bool ChaosWarhounds::s_registered = false;

    ChaosWarhounds::ChaosWarhounds(Greatfray fray, int numModels, int points) :
            BeastsOfChaosBase("Chaos Warhounds", 10, g_wounds, 4, 6, false, points),
            m_slaveringJaws(Weapon::Type::Melee, "Slavering Jaws", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, CHAOS_WARHOUNDS};
        m_weapons = {&m_slaveringJaws};

        setGreatfray(fray);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_slaveringJaws);
            addModel(model);
        }
    }

    Unit *ChaosWarhounds::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);

        return new ChaosWarhounds(fray, numModels, ComputePoints(parameters));
    }

    void ChaosWarhounds::Init() {
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
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Chaos Warhounds", factoryMethod);
        }
    }

    int ChaosWarhounds::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int ChaosWarhounds::rollRunDistance() {
        // Outrunners of Chaos
        return 6 + runModifier();
    }

} // namespace BeastsOfChaos