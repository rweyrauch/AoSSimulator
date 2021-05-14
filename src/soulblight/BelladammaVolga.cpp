/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/BelladammaVolga.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 50;
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 0;

    bool BelladammaVolga::s_registered = false;

    Unit *BelladammaVolga::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int BelladammaVolga::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void BelladammaVolga::Init() {

    }

    BelladammaVolga::BelladammaVolga(Legion legion, Lore lore, bool isGeneral) :
            SoulblightBase(legion, "Belladamma Volga", 10, g_wounds, 10, 4, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
