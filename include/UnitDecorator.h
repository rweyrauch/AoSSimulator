/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "UnitModifierInterface.h"

class UnitDecorator : public UnitModifierInterface {
public:
    explicit UnitDecorator(UnitModifierInterface* interface) :
        m_interface(interface) {

    }

    ~UnitDecorator() override = default;

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override {
        return m_interface->extraAttacks(attackingModel, weapon, target);
    }

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override {
        return m_interface->generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int toHitModifier(const Weapon *weapon, const Unit *target) const override {
        return m_interface->toHitModifier(weapon, target);
    }

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override {
        return m_interface->targetHitModifier(weapon, attacker);
    }

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override {
        return m_interface->toHitRerolls(weapon, target);
    }

    int toWoundModifier(const Weapon *weapon, const Unit *target) const override {
        return m_interface->toWoundModifier(weapon, target);
    }

    int targetWoundModifier(const Weapon *weapon, const Unit *attacker) const override {
        return m_interface->targetWoundModifier(weapon, attacker);
    }

    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override {
        return m_interface->toWoundRerolls(weapon, target);
    }

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override {
        return m_interface->weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll, int woundRoll) const override {
        return m_interface->targetAttackDamageModifier(wounds, attacker, hitRoll, woundRoll);
    }

    int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override {
        return m_interface->weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int toSaveModifier(const Weapon *weapon) const override {
        return m_interface->toSaveModifier(weapon);
    }

    int targetSaveModifier(const Weapon *weapon, const Unit *attacker) const override {
        return m_interface->targetSaveModifier(weapon, attacker);
    }

    Rerolls toSaveRerolls(const Weapon *weapon) const override {
        return m_interface->toSaveRerolls(weapon);
    }

    int braveryModifier() const override {
        return m_interface->braveryModifier();
    }

    Rerolls battleshockRerolls() const override {
        return m_interface->battleshockRerolls();
    }

    int castingModifier() const override {
        return m_interface->castingModifier();
    }

    Rerolls castingRerolls() const override {
        return m_interface->castingRerolls();
    }

    int unbindingModifier() const override {
        return m_interface->unbindingModifier();
    }

    int woundModifier() const override {
        return m_interface->woundModifier();
    }

    int moveModifier() const override {
        return m_interface->moveModifier();
    }

    int runModifier() const override {
        return m_interface->runModifier();
    }

    Rerolls runRerolls() const override {
        return m_interface->runRerolls();
    }

    int chargeModifier() const override {
        return m_interface->chargeModifier();
    }

    Rerolls chargeRerolls() const override {
        return m_interface->chargeRerolls();
    }

protected:
    UnitModifierInterface* m_interface = nullptr;

};
