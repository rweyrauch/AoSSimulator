/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>

class Unit;
class Spell;
class Prayer;

namespace OgorMawtribes {

    Spell *CreateLore(Lore which, Unit *caster);
    ::Prayer* CreatePrayer(Prayer which, Unit* priest);

} // namespace OgorMawtribes
