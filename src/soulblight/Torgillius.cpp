/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Torgillius.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool TorgilliusTheChamberlain::s_registered = false;

    Unit *TorgilliusTheChamberlain::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int TorgilliusTheChamberlain::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void TorgilliusTheChamberlain::Init() {

    }

    TorgilliusTheChamberlain::TorgilliusTheChamberlain(Legion legion, Lore lore, bool isGeneral) :
        SoulblightBase(legion, "Torgillius The Chamberlain", 5, g_wounds, 10, 6, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
