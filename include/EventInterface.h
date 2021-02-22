/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "AgeOfSigmarSim.h"

class Unit;
class Spell;

class  EventInterface {
public:
    EventInterface() = default;

    virtual ~EventInterface() = default;


    virtual void onRestore() {}

    virtual void onSlain() {}
    virtual void onEnemySlain(const Unit* unit) {}

    virtual void onModelSlain(Wounds::Source source) {}

    virtual void onWounded() {}

    virtual void onRan() {}

    virtual void onFlee(int numFled) {}

    virtual void onCharged() {}

    virtual void onUnboundSpell(Unit *caster, int castRoll) {}

    virtual void onCastSpell(const Spell *spell, const Unit *target) {}


    virtual void onBeginRound(int battleRound) {}

    virtual void onEndRound(int battleRound) {}

    virtual void onBeginTurn(int battleRound) {}

    virtual void onEndTurn(int battleRound) {}

    virtual void onStartHero(PlayerId player) {}

    virtual void onEndHero(PlayerId player) {}

    virtual void onStartMovement(PlayerId player) {}

    virtual void onEndMovement(PlayerId player) {}

    virtual void onStartShooting(PlayerId player) {}

    virtual Wounds onEndShooting(PlayerId player) { return {0, 0}; }

    virtual void onEndCharge(PlayerId player) {}

    virtual void onStartCombat(PlayerId player) {}

    virtual Wounds onEndCombat(PlayerId player) { return {0, 0}; }

    virtual void onStartBattleshock(PlayerId player) {}

    virtual void onEndBattleshock(PlayerId player) {}
};
