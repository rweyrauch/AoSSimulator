/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SONSOFBEHEMATPRIVATE_H
#define SONSOFBEHEMATPRIVATE_H

#include <behemat/SonsOfBehemat.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace SonsOfBehemat {

    constexpr std::array<int, 1> g_commandTrait = {
            to_integer(CommandTrait::None),
    };

    constexpr std::array<int, 1> g_artefact = {
            to_integer(Artefact::None),
    };

} // namespace SonsOfBehemat

#endif //SONSOFBEHEMATPRIVATE_H