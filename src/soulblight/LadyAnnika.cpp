/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/LadyAnnika.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 30;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool LadyAnnika::s_registered = false;

    Unit *LadyAnnika::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int LadyAnnika::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void LadyAnnika::Init() {

    }

    LadyAnnika::LadyAnnika(Legion legion, bool isGeneral) :
            SoulblightBase(legion, "Lady Annika The Thirsting Blade", 6, g_wounds, 10, 4, false, g_pointsPerUnit) {

    }

} // namespace Soulblight
