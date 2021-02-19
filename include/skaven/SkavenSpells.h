/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "skaven/Skaventide.h"
#include <Spell.h>

namespace Skaven {

    DamageSpell *CreateWarpLightning(Unit *caster);

    AreaOfEffectSpell *CreateWarpLightningStorm(Unit *caster);

    Spell* CreateLore(Lore which, Unit* caster);
}
