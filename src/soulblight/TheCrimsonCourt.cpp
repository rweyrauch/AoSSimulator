/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/TheCrimsonCourt.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 0;

    bool TheCrimsonCourt::s_registered = false;

    Unit *TheCrimsonCourt::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int TheCrimsonCourt::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void TheCrimsonCourt::Init() {

    }

    TheCrimsonCourt::TheCrimsonCourt(CursedBloodline legion):
        SoulblightBase(legion, "The Crimson Court", 6, g_wounds, 10, 4, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
