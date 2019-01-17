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
    virtual int move() const { return m_move; }
    int bravery() const { return m_bravery; }
    int save() const { return m_save; }
    bool fly() const { return m_fly; }

    void addModel(const Model& model);

    void restore();
    void beginTurn();

    // Phase functions (these are the 'think' functions that decide what this unit
    // is to do in each of the phases.
    virtual void hero(PlayerId player) {}
    virtual void movement(PlayerId player);
    virtual void shooting(PlayerId player) {}
    virtual void charge(PlayerId player) {}
    virtual void combat(PlayerId player) {}
    virtual void battleshock(PlayerId player) {}

    bool formation(int ranks);
    bool position(float x, float y, float z = 0.0f);
    void getPosition(float& x, float& y, float& z) const;

    float distanceTo(const Unit* unit) const;
    float distanceBetween(const Model* model, const Unit* unit) const;

    Wounds shoot(int numAttackingModels, Unit* targetUnit, int& numSlain);
    Wounds fight(int numAttackingModels, Unit* targetUnit, int& numSlain);
    int applyBattleshock();

    Wounds computeDamage(const WoundingHits& woundingHits, int mortalWounds, const Weapon *weapon);

    int applyDamage(const Wounds& totalWounds);
    int remainingModels() const;
    int remainingWounds() const;

    bool addKeyword(Keyword word);
    bool hasKeyword(Keyword word) const;

    void buffToHit(int modifier) { m_toHitBuff = modifier; }
    void buffToHitMissile(int modifier) { m_toHitBuffMissile = modifier; }

protected:

    Unit(const std::string& name, int move, int wounds, int bravery, int save, bool fly);

    virtual int toHitModifier(const Weapon* weapon, const Unit* unit) const { return m_toHitBuff; }
    virtual Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const { return NoRerolls; }

    virtual int toWoundModifier(const Weapon* weapon, const Unit* unit) const { return 0; }
    virtual Rerolls toWoundRerolls(const Weapon* weapon, const Unit* unit) const { return NoRerolls; }

    virtual int toHitModifierMissile(const Weapon* weapon, const Unit* unit) const { return m_toHitBuffMissile; }
    virtual Rerolls toHitRerollsMissile(const Weapon* weapon, const Unit* unit) const { return NoRerolls; }

    virtual int toWoundModifierMissile(const Weapon* weapon, const Unit* unit) const { return 0; }
    virtual Rerolls toWoundRerollsMissile(const Weapon* weapon, const Unit* unit) const { return NoRerolls; }

    virtual int toSaveModifier(const Weapon* weapon) const { return 0; }
    virtual Rerolls toSaveRerolls(const Weapon* weapon) const { return NoRerolls; }

    virtual int battlshockModifier() const;
    virtual Rerolls battleshockRerolls() const { return NoRerolls; }

    virtual int extraAttacks(const Weapon* weapon) const { return 0; }
    virtual int extraAttacksMissile(const Weapon* weapon) const { return 0; }

    virtual Hits applyHitModifiers(const Weapon* weapon, const Unit* unit, const Hits& hits) const { return hits; }
    virtual int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits) const { return 0; }

    virtual void onSlain() {}
    virtual void onWounded() {}

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

    int m_ranks = 1;
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
