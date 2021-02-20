/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Spell.h>
#include <sylvaneth/SylvanethBase.h>

namespace Sylvaneth {

    Spell *CreateMetamorphosis(Unit *caster);

    Spell *CreateUnleashSpites(Unit *caster);

    Spell *CreatePrimalTerror(Unit *caster);

    Spell *CreateTheReaping(Unit *caster);

    Spell *CreateLore(Lore lore, Unit *caster);
}
