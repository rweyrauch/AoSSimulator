/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SYLVANETHSPELLS_H
#define SYLVANETHSPELLS_H

#include <Spell.h>

Spell *CreateMetamorphosis(Unit *caster);

Spell *CreateUnleashSpites(Unit *caster);

Spell *CreatePrimalTerror(Unit *caster);

Spell *CreateTheReaping(Unit *caster);

// Lore of the Deepwood
Spell *CreateThroneOfVines(Unit *caster);

Spell *CreateRegrowth(Unit *caster);

Spell *CreateTheDwellersBelow(Unit *caster);

Spell *CreateDeadlyHarvest(Unit *caster);

Spell *CreateVerdurousHarmony(Unit *caster);

Spell *CreateTreesong(Unit *caster);

#endif //SYLVANETHSPELLS_H