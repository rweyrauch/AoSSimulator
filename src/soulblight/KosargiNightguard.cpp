/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/KosargiNightguard.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 2;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 1 * g_pointsPerBlock;

    bool KosargiNightguard::s_registered = false;

    Unit *KosargiNightguard::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new KosargiNightguard(numModels, ComputePoints(parameters));
    }

    int KosargiNightguard::ComputePoints(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void KosargiNightguard::Init() {
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
            s_registered = UnitFactory::Register("Kosargi Nightguard", factoryMethod);
        }
    }

    KosargiNightguard::KosargiNightguard(int numModels, int points) :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Kosargi Nightguard", 5, g_wounds, 10, 5, false, points) {

        m_keywords = {DEATH, OGOR, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, DEADWALKERS, KOSARGI_NIGHTGUARD};
        m_weapons = {&m_bardiche};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_bardiche);
            addModel(model);
        }
    }

} // namespace Soulblight
