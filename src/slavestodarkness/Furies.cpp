/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Furies.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 6;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 500;

    bool Furies::s_registered = false;

    Unit *Furies::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new Furies(legion, numModels, ComputePoints(parameters));
    }

    void Furies::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Furies::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Furies::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Furies", factoryMethod);
        }
    }

    Furies::Furies(DamnedLegion legion, int numModels, int points) :
            SlavesToDarknessBase(legion, "Furies", 12, g_wounds, 10, NoSave, true, points) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, FURIES};
        m_weapons = {&m_daggerAndClaws};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_daggerAndClaws);
            addModel(model);
        }
    }

    int Furies::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //SlavesToDarkness