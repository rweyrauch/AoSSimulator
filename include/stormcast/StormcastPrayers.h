/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Prayer.h>

namespace StormcastEternals {

    Prayer *CreateSanction(Unit *source);

    Prayer *CreateHealingStorm(Unit *source);

    Prayer *CreateLightningStorm(Unit *source);

} // namespace StormcastEternals
