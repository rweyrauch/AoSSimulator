/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/SpiritStorm.h>

int SpiritStorm::getDamage(int castingRoll) const
{
    return DamageSpell::getDamage(castingRoll);
}
