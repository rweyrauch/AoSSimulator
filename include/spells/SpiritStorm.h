/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIRITSTORM_H
#define SPIRITSTORM_H

#include <Spell.h>

class SpiritStorm : public DamageSpell
{
public:
    explicit SpiritStorm(Unit* caster) :
        DamageSpell(caster, "Spirit Storm", 7, 18.0f, 1)
    {}

protected:

    int getDamage(int castingRoll) const override;
};

#endif //SPIRITSTORM_H