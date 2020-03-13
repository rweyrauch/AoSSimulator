/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKAVENSPELLS_H
#define SKAVENSPELLS_H

#include <Spell.h>

DamageSpell* CreateWarpLightning(Unit* caster);
AreaOfEffectSpell* CreateWarpLightningStorm(Unit* caster);

#endif //SKAVENSPELLS_H