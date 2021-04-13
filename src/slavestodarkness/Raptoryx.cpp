/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Raptoryx.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 6;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 450;

    bool Raptoryx::s_registered = false;

    Unit *Raptoryx::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new Raptoryx(legion, numModels);
    }

    void Raptoryx::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Raptoryx::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Raptoryx::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Raptoryx", factoryMethod);
        }
    }

    Raptoryx::Raptoryx(DamnedLegion legion, int numModels) :
            SlavesToDarknessBase("Raptoryx", 10, g_wounds, 6, NoSave, true) {
        m_keywords = {CHAOS, SLAVES_TO_DARKNESS, RAPTORYX};
        m_weapons = {&m_beakAndTalons};

        setDamnedLegion(legion);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_beakAndTalons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    int Raptoryx::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int extra = SlavesToDarknessBase::extraAttacks(attackingModel, weapon, target);
        // Crazed Flock
        if (m_charged) {
            extra += 1;
        }
        return extra;
    }

    int Raptoryx::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //SlavesToDarkness