/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/BeastsOfNurgle.h>
#include "NurglePrivate.h"
#include <UnitFactory.h>
#include <Board.h>

namespace Nurgle {
    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool BeastsOfNurgle::s_registered = false;

    BeastsOfNurgle::BeastsOfNurgle(PlagueLegion legion, int numModels, int points) :
            NurgleBase(legion,"Beasts of Nurgle", 5, g_wounds, 10, 5, false, points) {
        m_keywords = {CHAOS, DAEMON, NURGLE, BEASTS_OF_NURGLE};
        m_weapons = {&m_limbsAndMaw, &m_tentaclesAndTongue};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_limbsAndMaw);
            model->addMeleeWeapon(&m_tentaclesAndTongue);
            addModel(model);
        }

        // Attention Seekers
        m_runAndCharge = true;
        m_retreatAndCharge = true;
    }

    Unit *BeastsOfNurgle::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new BeastsOfNurgle(legion, numModels, ComputePoints(parameters));
    }

    void BeastsOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BeastsOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    BeastsOfNurgle::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Beasts of Nurgle", factoryMethod);
        }
    }

    Wounds BeastsOfNurgle::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    int BeastsOfNurgle::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nurgle

