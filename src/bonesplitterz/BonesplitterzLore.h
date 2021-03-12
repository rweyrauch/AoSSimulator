/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <bonesplitterz/Bonesplitterz.h>

class Unit;

class Spell;

namespace Bonesplitterz {

    Spell *CreateLore(Lore which, Unit *caster);

} // namespace Bonesplitterz
