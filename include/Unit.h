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
#include <functional>

class Unit
{
public:
    Unit() = default;
    virtual ~Unit() = default;

    //
    // Immutable unit attributes.
    //
    const std::string& name() const { return m_name; }
    int wounds() const { return m_wounds; }
    virtual int move() const { return m_move; }
    int bravery() const { return m_bravery; }
    int save() const { return m_save; }
    bool fly() const { return m_fly; }
    bool ignoreRend() const { return m_ignoreRend; }
    int points() const { return m_points; }
    Role role() const { return m_battleFieldRole; }

    void setOwningPlayer(PlayerId player) { m_owningPlayer = player; }
    PlayerId owningPlayer() const { return m_owningPlayer; }

    int numSpells() const { return m_numSpells; }
    int numUnbinds() const { return m_numUnbinds; }
    int numPrayers() const { return m_numPrayers; }

    void setPoints(int points) { m_points = points; }
    void addModel(const Model& model);

    void restore();
    void beginTurn(int battleRound);

    // Phase functions (these are the 'think' functions that decide what this unit
    // is to do in each of the phases.
    virtual void hero(PlayerId player) {}
    virtual void movement(PlayerId player);
    virtual void shooting(PlayerId player);
    virtual void charge(PlayerId player);
    virtual void combat(PlayerId player);
    virtual void battleshock(PlayerId player);

    bool formation(int ranks);
    bool setPosition(const Math::Point3& pos, const Math::Vector3& orientation);
    const Math::Point3& position() const { return m_position; };

    float distanceTo(const Unit* unit) const;
    float distanceBetween(const Model* model, const Unit* unit) const;

    Wounds shoot(int numAttackingModels, Unit* targetUnit, int& numSlain);
    Wounds shoot(int &numSlain);
    Wounds fight(int numAttackingModels, Unit* targetUnit, int& numSlain);
    Wounds fight(PlayerId player, int& numSlain);

    int applyBattleshock();

    Wounds computeDamage(const WoundingHits& woundingHits, int mortalWounds,
        const Weapon *weapon, Wounds& woundsReturned);

    int applyDamage(const Wounds& totalWounds);
    int remainingModels() const;
    int remainingWounds() const;

    bool addKeyword(Keyword word);
    bool hasKeyword(Keyword word) const;

    void buffToHit(int modifier) { m_toHitBuff = modifier; }
    void buffToHitMissile(int modifier) { m_toHitBuffMissile = modifier; }

    void setShootingTarget(Unit* unit) { m_shootingTarget = unit; }
    void setMeleeTarget(Unit* unit) { m_meleeTarget = unit; }

    Unit* shootingTarget() { return m_shootingTarget; }
    Unit* meleeTarget() { return m_meleeTarget; }

    bool charged() const { return m_charged; }
    bool ran() const { return m_ran; }

    std::vector<Model>::const_iterator modelBegin() const { return m_models.begin(); }
    std::vector<Model>::const_iterator modelEnd() const { return m_models.end(); }

    int slay(int numModels);

    float basesizeInches() const { return m_basesize_mm / 25.4f; }

    virtual void visitWeapons(std::function<void(const Weapon*)>& visitor) = 0;

protected:

    Unit(const std::string& name, int move, int wounds, int bravery, int save, bool fly);

    /*
     * To-hit modifier (buffs) when this unit uses the given weapon to attack the target.
     */
    virtual int toHitModifier(const Weapon* weapon, const Unit* target) const { return m_toHitBuff; }

    /*
     * Target to-hit modifier (debuff) when the attacker unit targets this unit using the given weapon.
     */
    virtual int targetHitModifier(const Weapon* weapon, const Unit* attacker) const { return 0; }

    /*
     * To-hit rerolls when this unit uses the given weapon to attack the target.
     */
    virtual Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const { return NoRerolls; }

    /*
     * To-wound modifier (buffs) when this unit uses the given weapon to attack the target.
     */
    virtual int toWoundModifier(const Weapon* weapon, const Unit* target) const { return 0; }

    /*
     * Target to-wound modifier (debuff) when the attacker targets this unit using the given weapon.
     */
    virtual int targetWoundModifier(const Weapon* weapon, const Unit* attacker) const { return 0; }

    /*
     * To-wound rerolls when this unit use the given weapon to attack the target.
     */
    virtual Rerolls toWoundRerolls(const Weapon* weapon, const Unit* target) const { return NoRerolls; }

    virtual int toSaveModifier(const Weapon* weapon) const { return 0; }
    virtual Rerolls toSaveRerolls(const Weapon* weapon) const { return NoRerolls; }

    virtual int battlshockModifier() const;
    virtual Rerolls battleshockRerolls() const { return NoRerolls; }

    virtual int rollBattleshock() const;

    virtual int castingModifier() const { return 0; }
    virtual int unbindingModifier() const { return 0; }

    virtual int extraAttacks(const Weapon* weapon) const { return 0; }

    virtual Hits applyHitModifiers(const Weapon* weapon, const Unit* unit, const Hits& hits) const { return hits; }
    virtual int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits) { return 0; }

    virtual int runModifier() const { return 0; }
    virtual Rerolls runRerolls() const { return NoRerolls; }
    virtual int chargeModifier() const { return 0; }
    virtual Rerolls chargeRerolls() const { return NoRerolls; }

    virtual void onRestore() {}
    virtual void onBeginTurn(int battleRound) {}
    virtual void onSlain() {}
    virtual void onWounded() {}
    virtual void onRan() {}
    virtual void onCharged() {}
    virtual void onStartShooting(PlayerId player) {}
    virtual void onStartCombat(PlayerId player) {}
    virtual void onEndCombat(PlayerId player) {}
    virtual void onFlee(int numFled) {}

    int rollRunDistance() const;
    int rollChargeDistance() const;

    virtual Wounds computeReturnedDamage(const Weapon* weapon, const Dice::RollResult& saveRolls) const { return {0, 0}; }

    virtual Wounds applyWoundSave(const Wounds& wounds) { return wounds; }

protected:
    std::string m_name = "";
    int m_move = 0;
    int m_wounds = 0;
    int m_bravery = 0;
    int m_save = 0;
    float m_basesize_mm = 0.0f;
    int m_points = 0;
    Role m_battleFieldRole = Role::Other;

    std::vector<Keyword> m_keywords;

    bool m_fly = false;
    bool m_runAndShoot = false;
    bool m_runAndCharge = false;
    bool m_retreatAncCharge = false;
    bool m_ignoreRend = false;

    PlayerId m_owningPlayer = PlayerId::None;

    int m_numSpells = 0;
    int m_numUnbinds = 0;
    int m_numPrayers = 0;

    int m_ranks = 1;
    std::vector<Model> m_models;
    Math::Point3 m_position = {0.0f, 0.0f, 0.0f};
    Math::Vector3 m_orientation = {1.0f, 0.0f, 0.0f};

    int m_battleRound = 0;
    int m_modelsSlain = 0;
    bool m_ran = false;
    bool m_charged = false;
    bool m_moved = false;
    int m_toHitBuff = 0;
    int m_toHitBuffMissile = 0;

    Unit* m_shootingTarget = nullptr;
    Unit* m_meleeTarget = nullptr;

    bool m_verbose = false;

    enum BuffableAttribute
    {
        MoveDistance = 0,
        ChargeDistance,
        ToHit,
        ToWound,
        ToSave,
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

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override {}
};

#endif //UNIT_H
