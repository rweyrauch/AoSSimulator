/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Prayer.h>
#include <fyreslayers/Fyreslayer.h>

namespace Fyreslayers {

    Prayer *CreateZharrgrimBlessings(Blessing which, Unit *priest);

} // namespace Fyreslayers
