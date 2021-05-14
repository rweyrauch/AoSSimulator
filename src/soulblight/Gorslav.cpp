/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Gorslav.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 0;

    bool GorslavTheGravekeeper::s_registered = false;

    Unit *GorslavTheGravekeeper::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int GorslavTheGravekeeper::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void GorslavTheGravekeeper::Init() {

    }

    GorslavTheGravekeeper::GorslavTheGravekeeper(Legion legion, bool isGeneral) :
        SoulblightBase(legion, "Gorslav the Gravekeeper", 4, g_wounds, 10, 6, false, g_pointsPerUnit) {

    }
} // namespace Soulblight
