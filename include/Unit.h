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
#include <list>
#include <string>
#include <functional>
#include <Weapon.h>
#include <Model.h>
#include <UnitStatistics.h>

#include <Spell.h>
#include <Prayer.h>
#include <CommandAbility.h>

class Unit
{
public:
    Unit() = default;

    virtual ~Unit() = default;

    //
    // Immutable unit attributes.
    //
    const std::string &name() const { return m_name; }

    int wounds() const { return m_wounds; }

    virtual int move() const { return m_move; }

    virtual int bravery() const { return m_bravery; }

    int save() const { return m_save; }

    bool fly() const { return m_fly; }

    bool ignoreRend() const { return m_ignoreRend; }

    int points() const { return m_points; }

    Role role() const { return m_battleFieldRole; }

    void setOwningPlayer(PlayerId player) { m_owningPlayer = player; }

    PlayerId owningPlayer() const { return m_owningPlayer; }

    int numSpells() const { return m_totalSpells; }

    int numUnbinds() const { return m_totalUnbinds; }

    int numPrayers() const { return m_totalPrayers; }

    void setPoints(int points) { m_points = points; }

    bool addKeyword(Keyword word);

    bool hasKeyword(Keyword word) const;

    /*!
     * Add a model to this unit.
     * @param model
     */
    void addModel(const Model &model);

    /*!
     * Restore a unit to its initial condition, as it was at the start of the battle.
     */
    void restore();

    void beginRound(int battleRound);

    void endRound(int battleRound);

    void beginTurn(int battleRound, PlayerId playerWithTurn);

    void endTurn(int battleRound);

    // Phase functions (these are the 'think' functions that decide what this unit
    // is to do in each of the phases.

    int hero(PlayerId player, int cpAvailable);

    void movement(PlayerId player);

    void shooting(PlayerId player);

    void charge(PlayerId player);

    void combat(PlayerId player);

    void battleshock(PlayerId player);

    //
    // Positioning
    //
    bool formation(int ranks);

    bool setPosition(const Math::Point3 &pos, const Math::Vector3 &orientation);

    const Math::Point3 &position() const { return m_position; };

    float distanceTo(const Unit *unit) const;

    float distanceBetween(const Model *model, const Unit *unit) const;

    int numModelsWithin(const Model *model, float range) const;

    //
    // Combat
    //
    Wounds shoot(int numAttackingModels, Unit *targetUnit, int &numSlain);

    Wounds shoot(int &numSlain);

    Wounds fight(int numAttackingModels, Unit *targetUnit, int &numSlain);

    Wounds fight(PlayerId player, int &numSlain);

    int applyBattleshock();

    bool makeSave(int woundRoll, const Weapon* weapon, int& saveRoll);

    int applyDamage(const Wounds &totalWounds);

    int initialModels() const { return (int)m_models.size(); }

    int remainingModels() const;

    int remainingWounds() const;

    int remainingPoints() const;

    int heal(int numWounds);

    void setShootingTarget(Unit *unit) { m_shootingTarget = unit; }

    void setMeleeTarget(Unit *unit) { m_meleeTarget = unit; }

    Unit *shootingTarget() { return m_shootingTarget; }

    Unit *meleeTarget() { return m_meleeTarget; }

    bool charged() const { return m_charged; }

    bool ran() const { return m_ran; }

    std::vector<Model>::const_iterator modelBegin() const { return m_models.begin(); }

    std::vector<Model>::const_iterator modelEnd() const { return m_models.end(); }

    int slay(int numModels);

    bool unbind(const Unit* caster, int castRoll);

    float basesizeInches() const { return m_basesize_mm / 25.4f; }

    virtual void visitWeapons(std::function<void(const Weapon *)> &visitor) = 0;

    const UnitStatistics &getStatistics() const { return m_statistics; }

    bool isGeneral() const { return m_isGeneral; }

    bool buffModifier(BuffableAttribute which, int modifier, Duration duration);
    bool buffReroll(BuffableAttribute which, Rerolls reroll, Duration duration);

protected:

    Unit(const std::string &name, int move, int wounds, int bravery, int save, bool fly);

    void attackWithWeapon(const Weapon* weapon, Unit* target, const Model& fromModel,
        Wounds& totalWoundsInflicted, Wounds& totalWoundsSuffered);

    int rerolling(int initialRoll, Rerolls reroll, Dice& dice) const;

    int useCommandAbility(int cpAvailable);
    void castSpell();
    void makePrayer();
    void doPileIn();

    void timeoutBuffs(Phase phase, PlayerId player);

protected:

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
    virtual int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const { return  1; }

    /*!
     * To-hit modifier (buffs) when this unit uses the given weapon to attack the target.
     * @param weapon Attacking with weapon
     * @param target Unit being attacked
     * @return To-hit roll modifier.
     */
    virtual int toHitModifier(const Weapon *weapon, const Unit *target) const;

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
    virtual Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const;

    /*!
     * To-wound modifier (buffs) when this unit uses the given weapon to attack the target.
     * @param weapon Attacking with weapon
     * @param target Unit being attacked
     * @return To-wound roll modifier.
     */
    virtual int toWoundModifier(const Weapon *weapon, const Unit *target) const;

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
    virtual Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const;

    /*!
     * Compute the weapon damage on the given target with the hit and wound rolls.
     * @param weapon Attacking with weapon
     * @param target Unit being attacked.
     * @param hitRoll Roll to-hit
     * @param woundRoll Roll to-wound
     * @return Weapon damage
     */
    virtual Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const { return {weapon->damage(), 0}; }

    /*!
     * Some units do mortal wounds for simply existing.
     * @param unit Unit being attacked.
     * @return Number of mortal wounds.
     */
    virtual int generateMortalWounds(const Unit *unit) { return 0; }

    virtual int toSaveModifier(const Weapon *weapon) const;

    virtual Rerolls toSaveRerolls(const Weapon *weapon) const;

    /*!
     * Does this unit need to take battleshock.
     * @return True if battleshock tests must be made for this unit.
     */
    virtual bool battleshockRequired() const { return true; }

    virtual int braveryModifier() const;

    virtual Rerolls battleshockRerolls() const;

    virtual int rollBattleshock() const;

    virtual void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const;

    virtual int castingModifier() const;

    virtual int unbindingModifier() const;

    virtual void restoreModels(int numModels) { }

    virtual int moveModifier() const;

    virtual int runModifier() const;

    virtual Rerolls runRerolls() const;

    virtual int chargeModifier() const;

    virtual Rerolls chargeRerolls() const;

    virtual void onRestore() {}

    virtual void onBeginRound(int battleRound) {}
    virtual void onEndRound(int battleRound) {}

    virtual void onBeginTurn(int battleRound) {}

    virtual void onSlain() {}

    virtual void onWounded() {}

    virtual void onRan() {}

    virtual void onCharged() {}

    virtual void onStartHero(PlayerId player) {}

    virtual void onStartShooting(PlayerId player) {}

    virtual void onStartCombat(PlayerId player) {}

    virtual void onEndCombat(PlayerId player) {}

    virtual void onFlee(int numFled) {}

    int rollRunDistance() const;

    int rollChargeDistance() const;

    virtual Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const { return {0, 0}; }

    virtual Wounds applyWoundSave(const Wounds &wounds) { return wounds; }

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
    bool m_retreatAndCharge = false;
    bool m_retreatAndShoot = false;
    bool m_ignoreRend = false;
    bool m_isGeneral = false;

    PlayerId m_owningPlayer = PlayerId::None;

    int m_totalSpells = 0;
    int m_totalUnbinds = 0;
    int m_totalPrayers = 0;

    int m_ranks = 1;
    std::vector<Model> m_models;
    Math::Point3 m_position = {0.0f, 0.0f, 0.0f};
    Math::Vector3 m_orientation = {1.0f, 0.0f, 0.0f};

    int m_battleRound = 0;
    int m_modelsSlain = 0;
    bool m_ran = false;
    bool m_charged = false;
    bool m_moved = false;
    bool m_canMove = true;

    int m_spellsCast = 0;
    int m_spellsUnbound = 0;
    int m_prayersAttempted = 0;

    std::vector<std::unique_ptr<Spell>> m_knownSpells;
    std::vector<std::unique_ptr<Prayer>> m_knownPrayers;
    std::vector<std::unique_ptr<CommandAbility>> m_commandAbilities;

    Unit *m_shootingTarget = nullptr;
    Unit *m_meleeTarget = nullptr;

    TurnRecord m_currentRecord;
    UnitStatistics m_statistics;

    // TODO: How best manage buff/debuffs?  Data or code?
    // How to manage buff/debuff lifecycle?
    std::list<ModifierBuff> m_attributeModifiers[NUM_BUFFABLE_ATTRIBUTES];
    std::list<RerollBuff> m_rollModifiers[NUM_BUFFABLE_ATTRIBUTES];
};

class CustomUnit : public Unit
{
public:
    CustomUnit(const std::string &name, int move, int wounds, int bravery, int save, bool fly);

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override {}
};

#endif //UNIT_H
