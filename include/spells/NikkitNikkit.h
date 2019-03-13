/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NIKKIT_H
#define NIKKIT_H

#include <Spell.h>

class NikkitNikkit : public DamageSpell
{
public:
    explicit NikkitNikkit(Unit* caster);

protected:
    int getDamage(int castingRoll) const override;
};

#endif// NIKKIT_H