/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DISPOSSESSEDPRIVATE_H
#define DISPOSSESSEDPRIVATE_H

#include <dispossessed/Dispossessed.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace Dispossessed {

    constexpr std::array<int, 6> g_grudge = {
            ToInteger(Grudge::Stuck_Up),
            ToInteger(Grudge::Speed_Merchants),
            ToInteger(Grudge::Monstrous_Cheaters),
            ToInteger(Grudge::Cowardly_Horders),
            ToInteger(Grudge::Shoddy_Craftsmanship),
            ToInteger(Grudge::Sneaky_Ambushers)
    };

} // namespace Dispossessed

#endif // DISPOSSESSEDPRIVATE_H