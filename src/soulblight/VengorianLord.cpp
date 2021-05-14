/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/VengorianLord.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 0;

    bool VengorianLord::s_registered = false;

    Unit *VengorianLord::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int VengorianLord::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void VengorianLord::Init() {

    }

    VengorianLord::VengorianLord(CursedBloodline legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
        SoulblightBase(legion, "Vengorian Lord", 12, g_wounds, 10, 3, true, g_pointsPerUnit) {

    }
} // namespace Soulblight
