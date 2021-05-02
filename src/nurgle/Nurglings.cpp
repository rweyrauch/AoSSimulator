/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Nurglings.h>
#include <UnitFactory.h>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Nurglings::s_registered = false;

    Nurglings::Nurglings(int points) :
            NurgleBase("Nurglings", 5, g_wounds, 10, 6, false, points),
            m_teeth(Weapon::Type::Melee, "Tiny Razor-sharp Teeth", 1, 5, 5, 5, 0, 1) {
        m_keywords = {CHAOS, DAEMON, NURGLE, NURGLINGS};
        m_weapons = {&m_teeth};
    }

    bool Nurglings::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_teeth);
            addModel(model);
        }

        return true;
    }

    Unit *Nurglings::Create(const ParameterList &parameters) {
        auto unit = new Nurglings(ComputePoints(parameters));
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

    void Nurglings::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Nurglings::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    Nurglings::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Nurglings", factoryMethod);
        }
    }

    int Nurglings::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nurgle

