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
            to_integer(Grudge::Stuck_Up),
            to_integer(Grudge::Speed_Merchants),
            to_integer(Grudge::Monstrous_Cheaters),
            to_integer(Grudge::Cowardly_Horders),
            to_integer(Grudge::Shoddy_Craftsmanship),
            to_integer(Grudge::Sneaky_Ambushers)
    };

} // namespace Dispossessed

#endif // DISPOSSESSEDPRIVATE_H