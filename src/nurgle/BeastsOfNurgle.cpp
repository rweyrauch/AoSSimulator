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

    BeastsOfNurgle::BeastsOfNurgle() :
            NurgleBase("Beasts of Nurgle", 5, g_wounds, 10, 5, false) {
        m_keywords = {CHAOS, DAEMON, NURGLE, BEASTS_OF_NURGLE};
        m_weapons = {&m_limbsAndMaw, &m_tentaclesAndTongue};
    }

    bool BeastsOfNurgle::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_limbsAndMaw);
            model->addMeleeWeapon(&m_tentaclesAndTongue);
            addModel(model);
        }

        // Attention Seekers
        m_runAndCharge = true;
        m_retreatAndCharge = true;

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BeastsOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new BeastsOfNurgle();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    int BeastsOfNurgle::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nurgle

