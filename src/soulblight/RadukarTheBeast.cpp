/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/RadukarTheBeast.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 0;

    bool RadukarTheBeast::s_registered = false;

    Unit *RadukarTheBeast::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int RadukarTheBeast::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void RadukarTheBeast::Init() {

    }

    RadukarTheBeast::RadukarTheBeast(CursedBloodline legion, bool isGeneral) :
        SoulblightBase(legion, "Radukar The Beast", 8, g_wounds, 10, 4, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
