/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/VyrkosBloodborn.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 1 * g_pointsPerBlock;

    bool VyrkosBloodborn::s_registered = false;

    Unit *VyrkosBloodborn::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new VyrkosBloodborn(numModels, ComputePoints(parameters));
    }

    int VyrkosBloodborn::ComputePoints(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void VyrkosBloodborn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Vyrkos Bloodborn", factoryMethod);
        }
    }

    VyrkosBloodborn::VyrkosBloodborn(int numModels, int points) :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Vyrkos Bloodborn", 10, g_wounds, 10, 6, false, points) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, VYRKOS_BLOOD_BORN};
        m_weapons = {&m_blade};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blade);
            addModel(model);
        }
    }

} // namespace Soulblight
