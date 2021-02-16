/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <sylvaneth/SylvanethSpells.h>
#include <Board.h>
#include <Unit.h>

class Metamorphosis : public DamageSpell {
public:
    explicit Metamorphosis(Unit *caster);

protected:

    int getDamage(int castingRoll) const override;
};

Metamorphosis::Metamorphosis(Unit *caster) :
    DamageSpell(caster, "Metamorphosis", 7, 16, 1) {
}

int Metamorphosis::getDamage(int castingRoll) const {
    Dice::RollResult result;
    Dice::RollD6(castingRoll, result);
    return result.rollsGE(4);
}

Spell *CreateMetamorphosis(Unit *caster) {
    return new Metamorphosis(caster);
}

class UnleashSpites : public AreaOfEffectSpell {
public:
    explicit UnleashSpites(Unit *caster) :
        AreaOfEffectSpell(caster, "Unleash Spites", 5, 0, 9, 1, 0) {}

protected:

    int getDamage(int castingRoll) const override {
        Dice::RollResult result;
        Dice::RollD6(castingRoll, result);
        return result.rollsGE(6);
    }
};


Spell *CreateUnleashSpites(Unit *caster) {
    return new UnleashSpites(caster);
}

Spell *CreatePrimalTerror(Unit *caster) {
    return nullptr;
}

Spell *CreateTheReaping(Unit *caster) {
    return nullptr;
}

Spell *CreateThroneOfVines(Unit *caster) {
    return nullptr;
}

Spell *CreateRegrowth(Unit *caster) {
    return new HealSpell(caster, "Regrowth", 5, 18, RAND_D6);
}

Spell *CreateTheDwellersBelow(Unit *caster) {
    return nullptr;
}

Spell *CreateDeadlyHarvest(Unit *caster) {
    return new AreaOfEffectSpell(caster, "Deadly Harvest", 6, 0, 3, RAND_D3, 0);
}

Spell *CreateVerdurousHarmony(Unit *caster) {
    return nullptr;
}

Spell *CreateTreesong(Unit *caster) {
    return nullptr;
}
