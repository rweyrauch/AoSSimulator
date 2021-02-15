/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Spell.h>
#include <dok/DaughterOfKhaine.h>

namespace DaughtersOfKhaine {

    Spell *CreateLore(Lore which, Unit *caster);

}
