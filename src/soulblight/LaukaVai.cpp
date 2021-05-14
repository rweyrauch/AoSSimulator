/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/LaukaVai.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 0;

    bool LaukaVai::s_registered = false;

    Unit *LaukaVai::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int LaukaVai::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void LaukaVai::Init() {

    }

    LaukaVai::LaukaVai(Legion legion, Lore lore, bool isGeneral) :
        SoulblightBase(legion, "Lauka Vai Mother of Nightmares", 12, g_wounds, 10, 3, true, g_pointsPerUnit) {

    }
} // namespace Soulblight
