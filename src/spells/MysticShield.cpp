/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/MysticShield.h>

MysticShield::MysticShield(Unit *caster) :
    Spell(caster, "Mystic Shield", 7, 18.0f)
{

}

bool MysticShield::cast(Unit *target)
{
    return false;
}
