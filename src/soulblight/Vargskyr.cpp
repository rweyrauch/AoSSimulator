/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Vargskyr.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 0;

    bool Vargskyr::s_registered = false;

    Unit *Vargskyr::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int Vargskyr::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void Vargskyr::Init() {

    }

    Vargskyr::Vargskyr(CursedBloodline legion) :
        SoulblightBase(legion, "Vargskyr", 8, g_wounds, 10, 5, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
