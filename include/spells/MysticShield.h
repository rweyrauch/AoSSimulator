/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MYSTICSHIELD_H
#define MYSTICSHIELD_H

#include <Spell.h>

class MysticShield : public Spell
{
public:
    explicit MysticShield(Unit *caster);

    bool cast(Unit* target) override;
    bool cast(float x, float y) override { return false; }

protected:

};
#endif// MYSTICSHIELD_H