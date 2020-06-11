/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/NikkitNikkit.h>

NikkitNikkit::NikkitNikkit(Unit *caster) :
        DamageSpell(caster, "Nikkit! Nikkit!", 8, 18.0, RAND_D3) {}

int NikkitNikkit::getDamage(int castingRoll) const {
    if (castingRoll >= 10) {
        // TODO: negates artefact on target
    }
    return Dice::rollD3();
}
