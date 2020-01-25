/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/SylvanethSpells.h>
#include <Board.h>
#include <Unit.h>

class Metamorphosis : public DamageSpell
{
public:
    explicit Metamorphosis(Unit* caster);

protected:
    int getDamage(int castingRoll) const override;
};

Metamorphosis::Metamorphosis(Unit *caster) :
    DamageSpell(caster, "Metamorphosis", 7, 16.0f, 1)
{
}

int Metamorphosis::getDamage(int castingRoll) const
{
   Dice dice;
   Dice::RollResult result;
   dice.rollD6(castingRoll, result);
   return result.rollsGE(4);
}

Spell* CreateMetamorphosis(Unit* caster)
{
    return new Metamorphosis(caster);
}

class UnleashSpites : public AreaOfEffectSpell
{
public:
    explicit UnleashSpites(Unit* caster) :
        AreaOfEffectSpell(caster, "Unleash Spites", 5, 0.0f, 9.0f, 1, 0) {}

protected:
    int getDamage(int castingRoll) const override
    {
        Dice dice;
        Dice::RollResult result;
        dice.rollD6(castingRoll, result);
        return result.rollsGE(6);
    }
};


Spell* CreateUnleashSpites(Unit* caster)
{
    return new UnleashSpites(caster);
}
