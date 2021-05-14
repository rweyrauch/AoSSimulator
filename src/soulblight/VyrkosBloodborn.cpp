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
    static const int g_pointsPerUnit = 0;

    bool VyrkosBloodborn::s_registered = false;

    Unit *VyrkosBloodborn::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int VyrkosBloodborn::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void VyrkosBloodborn::Init() {

    }

    VyrkosBloodborn::VyrkosBloodborn(Legion legion, int numModels) :
        SoulblightBase(legion, "Vyrkos Bloodborn", 10, g_wounds, 10, 6, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
