/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "Spell.h"
#include <tzeentch/TzeentchBase.h>

namespace Tzeentch {

    Spell *CreateBoltOfChange(Unit *caster);

    Spell *CreateLore(Lore which, Unit *caster);
}
