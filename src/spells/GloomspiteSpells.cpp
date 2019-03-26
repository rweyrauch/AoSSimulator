/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/GloomspiteSpells.h>

#include "spells/GloomspiteSpells.h"

DamageSpell *CreateVindictiveGlare(Unit *caster)
{
    return new DamageSpell(caster, "Vindictive Glare", 5, 12, RAND_D3);
}
