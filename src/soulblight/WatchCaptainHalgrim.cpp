/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/WatchCaptainHalgrim.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 0;

    bool WatchCaptainHalgrim::s_registered = false;

    Unit *WatchCaptainHalgrim::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int WatchCaptainHalgrim::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void WatchCaptainHalgrim::Init() {

    }

    WatchCaptainHalgrim::WatchCaptainHalgrim(CursedBloodline legion, bool isGeneral) :
        SoulblightBase(legion, "Watch Captain Halgrim", 4, g_wounds, 10, 4, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
