/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Kritza.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 30;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool KritzaTheRatPrince::s_registered = false;

    Unit *KritzaTheRatPrince::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int KritzaTheRatPrince::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void KritzaTheRatPrince::Init() {

    }

    KritzaTheRatPrince::KritzaTheRatPrince(CursedBloodline legion, Lore lore, bool isGeneral) :
            SoulblightBase(legion, "Kritza The Rat Prince", 6, g_wounds, 10, 4, false, g_pointsPerUnit) {

    }

} // namespace Soulblight
