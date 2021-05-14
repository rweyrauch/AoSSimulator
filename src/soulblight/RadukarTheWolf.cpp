/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/RadukarTheWolf.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 0;

    bool RadukarTheWolf::s_registered = false;

    Unit *RadukarTheWolf::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int RadukarTheWolf::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void RadukarTheWolf::Init() {

    }

    RadukarTheWolf::RadukarTheWolf(CursedBloodline legion, Lore lore, bool isGeneral) :
        SoulblightBase(legion, "Radukar The Wolf", 5, g_wounds, 10, 4, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
