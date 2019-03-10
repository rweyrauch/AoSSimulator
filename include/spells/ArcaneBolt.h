/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARCANEBOLT_H
#define ARCANEBOLT_H

#include <Spell.h>

class ArcaneBolt : public DamageSpell
{
public:
    explicit ArcaneBolt(Unit* caster);

protected:
    int getDamage(int castingRoll) const override;
};

#endif// ARCANEBOLT_H