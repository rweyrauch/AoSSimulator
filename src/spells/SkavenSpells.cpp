/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/SkavenSpells.h>
#include <Board.h>
#include <Unit.h>

DamageSpell *CreateWarpLightning(Unit *caster) {
    // TODO: Implement the 'warp-power accumulator' option.
    return new DamageSpell(caster, "Warp Lightning", 5, 13.0f, RAND_D3);
}

AreaOfEffectSpell *CreateWarpLightningStorm(Unit *caster) {
    // TODO: Implement the 'warp-power accumulator' option.
    // TODO: Storm is limited to D3 units (really D3 casts of WarpLightning)
    return new AreaOfEffectSpell(caster, "Warp Lightning Storm", 7, 0.0f, 13.0f, RAND_D3, 0);
}
