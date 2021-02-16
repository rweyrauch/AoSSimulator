/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>

class Unit;
class Spell;

namespace BeastsOfChaos {

    Spell *CreateLore(Lore which, Unit *caster);

} // namespace BeastsOfChaos
