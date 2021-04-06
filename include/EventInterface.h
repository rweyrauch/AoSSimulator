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
class Model;
class Spell;
class Prayer;
class CommandAbility;

class  EventInterface {
public:
    EventInterface() = default;

    virtual ~EventInterface() = default;


    virtual void onRestore() {}

    virtual void onFriendlyUnitSlain(const Unit *attacker) {}
    virtual void onEnemyUnitSlain(const Unit* enemyUnit) {}

    virtual void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {}
    virtual void onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) {}

    virtual void onWounded() {}

    virtual void onRan() {}

    virtual void onFlee(int numFled) {}

    virtual void onModelFled(const Model* model) {}

    virtual void onCharged() {}

    virtual void onUnboundSpell(Unit *caster, int castRoll) {}

    virtual void onCastSpell(const Spell *spell, const Unit *target) {}

    virtual void onPrayerMade(const Prayer *prayer, const Unit* target) {}

    virtual void onCommandAbilityUsed(const CommandAbility* ability, const Unit* target) {}

    virtual void onBeginRound(int battleRound) {}

    virtual void onEndRound(int battleRound) {}

    virtual void onBeginTurn(int battleRound, PlayerId player) {}

    virtual void onEndTurn(int battleRound, PlayerId player) {}

    virtual void onStartHero(PlayerId player) {}

    virtual void onEndHero(PlayerId player) {}

    virtual void onStartMovement(PlayerId player) {}

    virtual void onEndMovement(PlayerId player) {}

    virtual void onStartShooting(PlayerId player) {}

    virtual void onEndShooting(PlayerId player) {}

    virtual void onEndCharge(PlayerId player) {}

    virtual void onStartCombat(PlayerId player) {}

    virtual void onEndCombat(PlayerId player) {}

    virtual void onStartBattleshock(PlayerId player) {}

    virtual void onEndBattleshock(PlayerId player) {}
};
