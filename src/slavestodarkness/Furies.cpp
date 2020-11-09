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
        auto unit = new Furies();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    bool Furies::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_daggerAndClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Furies::Furies() :
            SlavesToDarknessBase("Furies", 12, g_wounds, 10, NoSave, true),
            m_daggerAndClaws(Weapon::Type::Melee, "Razor-sharp Dagger and Claws", 1, 2, 4, 3, -1, 1) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, FURIES};
        m_weapons = {&m_daggerAndClaws};
    }

    int Furies::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //SlavesToDarkness