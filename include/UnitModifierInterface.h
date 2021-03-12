/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once


#include "Weapon.h"

class Roster;
class Model;
class Unit;

class UnitModifierInterface {
public:
    UnitModifierInterface() = default;

    virtual ~UnitModifierInterface() = default;

    /*!
     * Some weapons generate extra attacks.
     * Examples include additional attacks when charging or when a minimum number of models are attacking.
     * @param weapon Attacking weapon
     * @return Number of additional attacks (not including the normal attack).
     */
    virtual int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const { return 0; }

    /*!
     * Some weapons/units generate extra hits based on the hit
     * @param unmodifiedHitRoll
     * @param weapon
     * @param unit
     * @return
     */
    virtual int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const { return 1; }

    /*!
     * To-hit modifier (buffs) when this unit uses the given weapon to attack the target.
     * @param weapon Attacking with weapon
     * @param target Unit being attacked
     * @return To-hit roll modifier.
     */
    virtual int toHitModifier(const Weapon *weapon, const Unit *target) const { return 0; }

    /*!
     * Target to-hit modifier (debuff) when the attacker unit targets this unit using the given weapon.
     * @param weapon Weapon used in the attack.
     * @param target Unit attacking this unit.
     * @return To-hit roll modifier.
     */
    virtual int targetHitModifier(const Weapon *weapon, const Unit *attacker) const { return 0; }

    /*!
     * To-hit rerolls when this unit uses the given weapon to attack the target.
     * @param weapon Attacking with weapon
     * @param target Unit being attacked
     * @return To-hit re-roll.
     */
    virtual Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const { return Rerolls::None; }

    /*!
     * To-wound modifier (buffs) when this unit uses the given weapon to attack the target.
     * @param weapon Attacking with weapon
     * @param target Unit being attacked
     * @return To-wound roll modifier.
     */
    virtual int toWoundModifier(const Weapon *weapon, const Unit *target) const { return 0; }

    /*!
     * Target to-wound modifier (debuff) when the attacker targets this unit using the given weapon.
     * @param weapon Weapon used in the attack.
     * @param target Unit attacking this unit.
     * @return To-wound roll modifier.
     */
    virtual int targetWoundModifier(const Weapon *weapon, const Unit *attacker) const { return 0; }

    /*!
     * To-wound re-rolls when this unit use the given weapon to attack the target.
     * @param weapon Attacking with weapon
     * @param target Unit being attacked
     * @return To-wound re-roll.
     */
    virtual Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const { return Rerolls::None; }

    /*!
     * Compute the weapon damage on the given target with the hit and wound rolls.
     * @param weapon Attacking with weapon
     * @param target Unit being attacked.
     * @param hitRoll Roll to-hit
     * @param woundRoll Roll to-wound
     * @return Weapon damage
     */
    virtual Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        return {weapon->damage(), 0};
    }

    /*!
     * Modifier to apply to damage for a single attack.
     * @param wounds Attack damage
     * @param attacker Attacking unit
     * @param hitRoll Unmodified roll to-hit
     * @param woundRoll Unmodified roll to-wound
     * @return Modified wounds
     */
    virtual Wounds targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll,
                                              int woundRoll) const { return wounds; }

    /*!
     * Compute the weapon rend against the given target with the hit and wound rolls.
     * @param weapon Attacking with weapon.
     * @param target Unit being attacked.
     * @param hitRoll Roll to-hit
     * @param woundRoll Roll to-wound
     * @return Weapon rend
     */
    virtual int weaponRend(const Weapon *weapon, const Unit *target,
                           int hitRoll, int woundRoll) const { return weapon->rend(); }

    virtual int toSaveModifier(const Weapon *weapon, const Unit* attacker) const { return 0; }

    virtual int targetSaveModifier(const Weapon *weapon, const Unit *attacker) const { return 0; }

    virtual Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const { return Rerolls::None; }

    [[nodiscard]] virtual int braveryModifier() const { return 0; }

    [[nodiscard]] virtual Rerolls battleshockRerolls() const { return Rerolls::None; }

    [[nodiscard]] virtual int castingModifier() const { return 0; }

    [[nodiscard]] virtual Rerolls castingRerolls() const { return Rerolls::None; }

    [[nodiscard]] virtual int unbindingModifier() const { return 0; }

    [[nodiscard]] virtual Rerolls unbindingRerolls() const { return Rerolls::None; }

    [[nodiscard]] virtual Rerolls prayerRerolls() const { return Rerolls::None; }

    [[nodiscard]] virtual int woundModifier() const { return 0; }

    [[nodiscard]] virtual int moveModifier() const { return 0; }

    [[nodiscard]] virtual int runModifier() const { return 0; }

    [[nodiscard]] virtual Rerolls runRerolls() const { return Rerolls::None; }

    [[nodiscard]] virtual int chargeModifier() const { return 0; }

    [[nodiscard]] virtual Rerolls chargeRerolls() const { return Rerolls::None; }
};
