/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNIT_H
#define UNIT_H

#include <memory>
#include <vector>
#include <string>
#include <Weapon.h>
#include <Model.h>

class Unit
{
public:
    Unit() = default;
    virtual ~Unit() = default;

    const std::string& name() const { return m_name; }
    int wounds() const { return m_wounds; }
    int move() const { return m_move; }
    int bravery() const { return m_bravery; }
    int save() const { return m_save; }
    bool fly() const { return m_fly; }

    void addModel(const Model& model);

    void beginTurn();

    // Phase functions (these are the 'think' functions that decide what this unit
    // is to do in each of the phases.
    virtual void hero() {}
    virtual void movement() {}
    virtual void shooting() {}
    virtual void charge() {}
    virtual void combat() {}
    virtual void battleshock() {}


    int shoot(int numAttackingModels, Unit& unit);
    int fight(int numAttackingModels, Unit& unit);
    int applyBattleshock();

    int computeDamage(int numWoundingHits, const Weapon &weapon);
    int applyDamage(int totalDamage);
    int remainingModels() const;
    int remainingWounds() const;

    bool addKeyword(Keyword word);
    bool hasKeyword(Keyword word) const;

    void buffToHit(int modifier) { m_toHitBuff = modifier; }
    void buffToHitMissile(int modifier) { m_toHitBuffMissile = modifier; }

protected:

    Unit(const std::string& name, int move, int wounds, int bravery, int save, bool fly);

    virtual int toHitModifier(const Unit& unit) const { return m_toHitBuff; }
    virtual Rerolls toHitRerolls(const Unit& unit) const { return NoRerolls; }

    virtual int toWoundModifier(const Unit& unit) const { return 0; }
    virtual Rerolls toWoundRerolls(const Unit& unit) const { return NoRerolls; }

    virtual int toHitModifierMissile(const Unit& unit) const { return m_toHitBuffMissile; }
    virtual Rerolls toHitRerollsMissile(const Unit& unit) const { return NoRerolls; }

    virtual int toWoundModifierMissile(const Unit& unit) const { return 0; }
    virtual Rerolls toWoundRerollsMissile(const Unit& unit) const { return NoRerolls; }

    virtual int toSaveModifier() const { return 0; }
    virtual Rerolls toSaveRerolls() const { return NoRerolls; }

    virtual int battlshockModifier() const { return 0; }
    virtual Rerolls battleshockRerolls() const { return NoRerolls; }

    virtual int extraAttacks() const { return 0; }
    virtual int extraAttacksMissile() const { return 0; }

    virtual HitModifier hitModifier() const { return NoExtraHits; }
    virtual HitModifier hitModifierMissile() const { return NoExtraHits; }

protected:
    std::string m_name = "";
    int m_move = 0;
    int m_wounds = 0;
    int m_bravery = 0;
    int m_save = 0;

    bool m_fly = false;
    bool m_runAndShoot = false;
    bool m_runAndCharge = false;
    bool m_ignoreRend = false;

    std::vector<Model> m_models;

    int m_modelsSlain = 0;
    int m_ran = false;
    int m_charged = false;
    int m_toHitBuff = 0;
    int m_toHitBuffMissile = 0;

    std::vector<Keyword> m_keywords;

    enum BuffableAttribute
    {
        MoveDistance = 0,
        ChargeDistance,
        ToHit_Combat,
        ToHit_Missile,
        ToWound_Combat,
        ToWound_Missile,
        ToSave_Combat,
        ToSave_Missile,
        Bravery,

        NUM_BUFFABLE_ATTRIBUTES
    };
    // TODO: How best manage buff/debuffs?  Data or code?
    // How to manage buff/debuff lifecycle?
    int m_attributeModifiers[NUM_BUFFABLE_ATTRIBUTES];
    Rerolls m_rollModifiers[NUM_BUFFABLE_ATTRIBUTES];
};

class CustomUnit : public Unit
{
public:
    CustomUnit(const std::string& name, int move, int wounds, int bravery, int save, bool fly);

};

#endif //UNIT_H
