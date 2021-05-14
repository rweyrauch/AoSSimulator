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
    static const int g_pointsPerUnit = 0;

    bool KosargiNightguard::s_registered = false;

    Unit *KosargiNightguard::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int KosargiNightguard::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void KosargiNightguard::Init() {

    }

    KosargiNightguard::KosargiNightguard(Legion legion, int numModels) :
        SoulblightBase(legion, "Kosargi Nightguard", 5, g_wounds, 10, 5, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
