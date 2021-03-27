/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>

class Unit;

class Spell;

namespace Slaanesh {

    Spell *CreateLore(Lore which, Unit *caster);

    Spell *CreateAcquiescence(Unit *caster);

} // namespace Slaanesh
