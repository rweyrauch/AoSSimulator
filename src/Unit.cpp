/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cfloat>
#include <algorithm>
#include <utility>
#include <Unit.h>
#include <Dice.h>
#include <Board.h>
#include <Roster.h>
#include <Think.h>

const double MAX_CHARGE_DISTANCE = 12.0;
const double MIN_CHARGE_DISTANCE = 3.0;

lsignal::signal<int(const Unit *)> Unit::s_globalMoveMod;
lsignal::signal<int(const Unit *)> Unit::s_globalRunMod;
lsignal::signal<int(const Unit *)> Unit::s_globalChargeMod;

lsignal::signal<int(const Unit *)> Unit::s_globalBraveryMod;
lsignal::signal<int(const Unit *, const Weapon *, const Unit *)> Unit::s_globalToHitMod;
lsignal::signal<int(const Unit *, const Weapon *, const Unit *)> Unit::s_globalToWoundMod;
lsignal::signal<int(const Unit *, const Weapon *)> Unit::s_globalSaveMod;

lsignal::signal<int(const Unit *, const Model *, const Weapon *, const Unit *)> Unit::s_globalAttackMod;

lsignal::signal<int(const Unit *)> Unit::s_globalCastMod;
lsignal::signal<int(const Unit *)> Unit::s_globalUnbindMod;

lsignal::signal<Rerolls(const Unit *, const Weapon *, const Unit *)> Unit::s_globalToHitReroll;
lsignal::signal<Rerolls(const Unit *, const Weapon *, const Unit *)> Unit::s_globalToWoundReroll;
lsignal::signal<Rerolls(const Unit *, const Weapon *, const Unit *)> Unit::s_globalSaveReroll;
lsignal::signal<Rerolls(const Unit *)> Unit::s_globalBattleshockReroll;

lsignal::signal<Rerolls(const Unit *)> Unit::s_globalRunReroll;
lsignal::signal<Rerolls(const Unit *)> Unit::s_globalChargeReroll;


static int accumulate(const std::vector<int> &v) {
    return std::accumulate(v.cbegin(), v.cend(), 0);
}

Wounds Unit::shoot(int numAttackingModels, Unit *targetUnit, int &numSlain) {
    if (m_ran && !canRunAndShoot()) {
        return {0, 0};
    }

    if ((numAttackingModels == -1) || (numAttackingModels > (int) m_models.size())) {
        numAttackingModels = (int) m_models.size();
    }

    Wounds totalDamage = {0, 0};
    Wounds totalDamageReflected = {0, 0};

    for (auto i = 0; i < numAttackingModels; i++) {
        auto model = m_models.at(i).get();
        if (model->fled() || model->slain()) continue;

        for (auto wip = model->missileWeaponBegin(); wip != model->missileWeaponEnd(); ++wip) {
            const Weapon *w = *wip;

            Wounds weaponDamage, reflectedDamage;
            attackWithWeapon(w, targetUnit, model, weaponDamage, reflectedDamage);

            totalDamage += weaponDamage;
            totalDamageReflected += reflectedDamage;
        }
    }

    // Apply all of the wounds to the target.  Target apply wound save and return
    // number of slain models.
    numSlain = targetUnit->applyDamage(totalDamage);

    if (targetUnit->remainingModels() == 0) {
        targetUnit->onSlain();
    }

    // apply returned damage to this unit
    int numSlainByReturnedDamage = applyDamage(totalDamageReflected);
    if (remainingModels() == 0) {
        onSlain();
    }

    m_currentRecord.m_woundsInflicted += totalDamage;
    m_currentRecord.m_woundsTaken += totalDamageReflected;
    m_currentRecord.m_enemyModelsSlain += numSlain;
    m_currentRecord.m_modelsSlain += numSlainByReturnedDamage;

    return totalDamage;
}


Wounds Unit::fight(int numAttackingModels, Unit *targetUnit, int &numSlain) {
    if ((numAttackingModels == -1) || (numAttackingModels > (int) m_models.size())) {
        numAttackingModels = (int) m_models.size();
    }

    doPileIn();

    Wounds totalDamage = {0, 0};
    Wounds totalDamageReflected = {0, 0};

    SimLog(Verbosity::Narrative, "%s attacking %s with %d models.\n", name().c_str(), targetUnit->name().c_str(),
           numAttackingModels);

    for (auto i = 0; i < numAttackingModels; i++) {
        const auto model = m_models.at(i).get();
        if (model->fled() || model->slain()) continue;

        for (auto wip = model->meleeWeaponBegin(); wip != model->meleeWeaponEnd(); ++wip) {
            const Weapon *w = *wip;

            Wounds weaponDamage, reflectedDamage;
            attackWithWeapon(w, targetUnit, model, weaponDamage, reflectedDamage);

            SimLog(Verbosity::Narrative, "\tModel[%d] attack with %s does damage {%d,%d}.\n",
                   i, w->name().c_str(), weaponDamage.normal, weaponDamage.mortal);

            totalDamage += weaponDamage;
            totalDamageReflected += reflectedDamage;
        }
    }

    // Some units do mortal wounds for just existing!  See Evocators for example.
    totalDamage.mortal += generateMortalWounds(targetUnit);

    // Apply all of the wounds to the target.  Target apply wound save and return
    // number of slain models.
    numSlain = targetUnit->applyDamage(totalDamage);

    int numSlainByReturnedDamage = applyDamage(totalDamageReflected);
    if (remainingModels() == 0) {
        onSlain();
    }

    m_currentRecord.m_woundsInflicted += totalDamage;
    m_currentRecord.m_woundsTaken += totalDamageReflected;
    m_currentRecord.m_enemyModelsSlain += numSlain;
    m_currentRecord.m_modelsSlain += numSlainByReturnedDamage;

    m_hasFought = true;

    return totalDamage;
}

int Unit::applyBattleshock() {
    if (!battleshockRequired()) return 0;
    if (m_modelsSlain <= 0) return 0;

    // TODO: implement rerolls for battleshock.
    auto roll = rollBattleshock();
    int numFled, numRestored;
    computeBattleshockEffect(roll, numFled, numRestored);

    // mark fleeing models
    int numFleeing = numFled;

    for (auto ip = m_models.rbegin(); ip != m_models.rend() && numFleeing > 0; ++ip) {
        Model *model = ip->get();
        if (model->fled() || model->slain()) continue;
        model->flee();
        numFleeing--;
    }

    if (numFled > 0)
        onFlee(numFled);

    m_currentRecord.m_numFled += numFled;

    // restore models
    if (numRestored > 0) {
        restoreModels(numRestored);
    }

    return numFled;
}

Unit::Unit(std::string name, int move, int wounds, int bravery, int save, bool fly) :
        m_name(std::move(name)),
        m_move(move),
        m_wounds(wounds),
        m_bravery(bravery),
        m_save(save),
        m_fly(fly),
        m_modelsSlain(0) {
}

void Unit::beginRound(int battleRound) {
    m_battleRound = battleRound;
    onBeginRound(battleRound);
}

void Unit::endRound(int battleRound) {
    onEndRound(battleRound);
}

void Unit::beginTurn(int battleRound, PlayerId playerWithTurn) {
    m_ran = false;
    m_charged = false;
    m_moved = false;
    m_hasFought = false;
    m_modelsSlain = 0;
    m_spellsCast = 0;
    m_prayersAttempted = 0;

    m_currentRecord.clear();

    m_currentRecord.m_playerWithTurn = playerWithTurn;
    m_currentRecord.m_round = battleRound;

    onBeginTurn(battleRound);
}

void Unit::endTurn(int /*battleRound*/) {
    m_statistics.record(m_currentRecord);
}

void Unit::addModel(Model *model) {
    m_models.push_back(std::unique_ptr<Model>(model));
    m_basesizeMm = (double) model->basesize();
}

int Unit::applyDamage(const Wounds &totalWoundsInflicted) {
    // apply wound/mortal wound save
    auto totalWounds = applyWoundSave(totalWoundsInflicted);

    // apply both normal and mortal wounds
    int totalDamage = totalWounds.normal + totalWounds.mortal;
    int numSlain = 0;
    for (const auto &model : m_models) {
        if (model->slain() || model->fled()) continue;

        auto wounds = model->woundsRemaining();
        auto remainingWounds = model->applyWound(totalDamage);
        if (totalDamage >= wounds) {
            totalDamage -= wounds;
        } else {
            totalDamage = 0;
        }
        if (remainingWounds == 0) {
            onModelSlain(totalWounds.source);
            numSlain++;
        }
    }

    onWounded();
    m_modelsSlain += numSlain;

    m_currentRecord.m_modelsSlain += numSlain;
    m_currentRecord.m_woundsTaken += totalWounds;

    if (remainingModels() == 0) {
        onSlain();
    }

    return numSlain;
}

int Unit::remainingModels() const {
    int models = 0;
    for (const auto &m : m_models) {
        if (!m->slain() && !m->fled())
            models++;
    }
    return models;
}

int Unit::remainingWounds() const {
    int wounds = 0;
    for (const auto &m : m_models) {
        if (!m->slain() && !m->fled())
            wounds += m->woundsRemaining();
    }
    return wounds;
}

bool Unit::hasKeyword(Keyword word) const {
    auto kw = std::find(m_keywords.begin(), m_keywords.end(), word);
    return (kw != m_keywords.end());
}

bool Unit::addKeyword(Keyword word) {
    m_keywords.push_back(word);
    return true;
}

void Unit::removeKeyword(Keyword word) {
    auto kw = std::find(m_keywords.begin(), m_keywords.end(), word);
    if (kw != m_keywords.end()) {
        m_keywords.erase(kw);
    }
}

int Unit::braveryModifier() const {
    int modifier = remainingModels() / 10 + s_globalBraveryMod(this, accumulate);
    for (auto bi : m_attributeModifiers[Bravery]) {
        modifier += bi.modifier;
    }
    return modifier;
}

void Unit::restore() {
    for (auto &m : m_models) {
        m->restore();
    }

    m_battleRound = 0;
    m_modelsSlain = 0;
    m_ran = false;
    m_charged = false;
    m_moved = false;
    m_canMove = true;

    m_spellsCast = 0;
    m_spellsUnbound = 0;
    m_prayersAttempted = 0;

    m_currentRecord.clear();
    m_statistics.reset();

    for (auto list : m_attributeModifiers) {
        list.clear();
    }
    for (auto list : m_rollModifiers) {
        list.clear();
    }
    for (auto list : m_movementRules) {
        list.clear();
    }

    onRestore();
}

void Unit::deploy(const Math::Point3 &pos, const Math::Vector3 &orientation) {

    formation(computeFormation());

    m_position = pos;

    const auto modelSpacing = std::min(1.0, basesizeInches() * 0.3);
    int unitW = (int) m_models.size() / m_ranks;
    double unitWidth = m_models.size() * (basesizeInches() + modelSpacing) / (double) m_ranks;

    Math::Vector3 up(0, 0, 1);
    Math::Vector3 left = Math::Vector3::Cross(up, orientation);
    left.normalize();

    int xi = 0;
    int yi = 0;

    Math::Vector3 dx = -left * (basesizeInches() + modelSpacing);
    Math::Vector3 dy = -orientation * (basesizeInches() + modelSpacing);

    Math::Vector3 pos0 = Math::Vector3(pos.x, pos.y, pos.z) + left * unitWidth / 2.0;
    Math::Vector3 curPos = pos0;

    // Pack models into block of [numModels / m_ranks, m_ranks]
    for (auto &m : m_models) {
        m->setPosition(Math::Point3(curPos.x, curPos.y, curPos.z));
        curPos += dx;
        xi++;
        if (xi >= unitW) {
            xi = 0;
            yi++;

            curPos = pos0;
            curPos += (double) yi * dy;
        }
    }
}

bool Unit::move(const Math::Point3 &pos, const Math::Vector3 &orientation) {
    m_position = pos;

    const auto modelSpacing = std::min(1.0, basesizeInches() * 0.3);
    int unitW = (int) m_models.size() / m_ranks;
    double unitWidth = m_models.size() * (basesizeInches() + modelSpacing) / (double) m_ranks;

    Math::Vector3 up(0, 0, 1);
    Math::Vector3 left = Math::Vector3::Cross(up, orientation);
    left.normalize();

    int xi = 0;
    int yi = 0;

    Math::Vector3 dx = -left * (basesizeInches() + modelSpacing);
    Math::Vector3 dy = -orientation * (basesizeInches() + modelSpacing);

    Math::Vector3 pos0 = Math::Vector3(pos.x, pos.y, pos.z) + left * unitWidth / 2.0;
    Math::Vector3 curPos = pos0;

    // Move models as a pack of [numModels / m_ranks, m_ranks]
    for (auto &m : m_models) {
        Board::Instance()->moveModel(*m, Math::Point3(curPos.x, curPos.y, curPos.z));

        xi++;
        curPos += dx;
        if (xi >= unitW) {
            xi = 0;
            yi++;

            curPos = pos0;
            curPos += (double) yi * dy;
        }
    }

    return true;
}

bool Unit::formation(int ranks) {
    if (ranks < 1 || ranks > (int) m_models.size())
        return false;

    m_ranks = ranks;
    return true;
}

double Unit::distanceTo(const Unit *unit) const {
    if (unit == nullptr || unit->remainingModels() == 0 || remainingModels() == 0)
        return 0;

    const auto pos = position();
    const auto unitPos = unit->position();

    double dist = pos.distance(unitPos);
    return std::max(0.0, dist - (basesizeInches() + unit->basesizeInches()) / 2.0);
}

double Unit::distanceTo(const Math::Point3 &point) const {
    const auto pos = position();

    double dist = pos.distance(point);
    return std::max(0.0, dist - basesizeInches() / 2.0);
}

double Unit::distanceBetween(const Model *model, const Unit *unit) const {
    if (model == nullptr || unit == nullptr || unit->remainingModels() == 0)
        return 0;

    const double tbs = unit->basesizeInches() / 2;

    const double x = model->x();
    const double y = model->y();

    auto minDist = DBL_MAX;

    // find closes model in target unit
    for (auto &m : unit->m_models) {
        const double tx = m->x();
        const double ty = m->y();

        const double dx = fabs(tx - x);
        const double dy = fabs(ty - y);

        auto dist = sqrt(dx * dx + dy * dy) - (basesizeInches() / 2 + tbs);
        if (dist < minDist) {
            minDist = dist;
        }
    }

    return std::max(0.0, minDist);
}

void Unit::hero(PlayerId player) {
    timeoutBuffs(Phase::Hero, player);

    onStartHero(player);

    if (player == owningPlayer()) {
        // Use command ability.
        useCommandAbility();

        // Cast spell
        castSpell();

        // Make prayer
        makePrayer();
    } else {
        // Opposing player's hero phase
    }
}

void Unit::movement(PlayerId player) {
    timeoutBuffs(Phase::Movement, player);

    onStartMovement(player);

    auto board = Board::Instance();

    m_moved = false;

    // Unit cannot move
    if (!m_canMove) return;

    auto weapon = m_models.front()->preferredWeapon();
    assert(weapon);

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);
    auto closestTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    if (closestTarget) {
        auto distance = distanceTo(closestTarget);
        double totalMoveDistance = 0;
        if (weapon && weapon->isMissile()) {
            const auto movement = (double) (move() + moveModifier());

            // get into range (run or not?)
            if (distance > (double) weapon->range() + movement) {
                // too far to get into range with normal move - run
                auto runDist = rollRunDistance();
                if ((runDist < 3) && (runRerolls() != NoRerolls))
                    runDist = rollRunDistance();
                totalMoveDistance = movement + runDist;
                m_ran = true;

                m_currentRecord.m_moved = movement;
                m_currentRecord.m_ran = runDist;
            } else if (distance > (double) weapon->range()) {
                // move toward unit
                totalMoveDistance = movement;

                m_currentRecord.m_moved = movement;
            } else {
                // already in range - stand still
                m_currentRecord.m_moved = 0;
            }
        } else {
            const auto movement = (double) (move() + moveModifier());

            if (distance <= MIN_CHARGE_DISTANCE) {
                // already in charge range - stand still
                m_currentRecord.m_moved = 0;
            }
            if (weapon && (distance > (double) weapon->range() + movement)) {
                // too far to get into range with normal move - run
                auto runDist = rollRunDistance();
                if ((runDist < 3) && (runRerolls() != NoRerolls))
                    runDist = rollRunDistance();
                totalMoveDistance = movement + runDist;
                m_ran = true;

                m_currentRecord.m_moved = movement;
                m_currentRecord.m_ran = runDist;
            } else {
                // normal move toward unit
                totalMoveDistance = std::min(movement, std::max(0.0, distance - MIN_CHARGE_DISTANCE));

                m_currentRecord.m_moved = totalMoveDistance;
            }
        }
        Math::Ray ray(position(), closestTarget->position());
        auto newPos = ray.point_at(totalMoveDistance);

        move(newPos, ray.get_direction());

        m_moved = (totalMoveDistance > 0.0);
    } else {
        // no target units - stand here confused!!!
        m_moved = false;
    }

    if (m_ran)
        onRan();

    onEndMovement(player);
}

int Unit::rollBattleshock() const {
    return Dice::rollD6();
}

Wounds Unit::shoot(int &numSlain) {
    if (m_shootingTarget == nullptr) {
        numSlain = 0;
        return {0, 0};
    }
    return shoot(-1, m_shootingTarget, numSlain);
}

Wounds Unit::fight(PlayerId player, int &numSlain) {
    if (m_meleeTarget == nullptr) {
        numSlain = 0;
        return {0, 0};
    }
    auto wounds = fight(-1, m_meleeTarget, numSlain);

    wounds += onEndCombat(player);

    return wounds;
}

void Unit::shooting(PlayerId player) {
    timeoutBuffs(Phase::Shooting, player);

    auto board = Board::Instance();

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    m_shootingTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    onStartShooting(player);
}

void Unit::combat(PlayerId player) {
    timeoutBuffs(Phase::Combat, player);

    auto board = Board::Instance();

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    m_meleeTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    onStartCombat(player);
}

void Unit::charge(PlayerId player) {
    timeoutBuffs(Phase::Charge, player);

    auto board = Board::Instance();

    auto weapon = m_models.front()->preferredWeapon();
    assert(weapon);
    if (weapon && weapon->isMissile()) return;

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);
    auto closestTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    if (m_ran && !canRunAndCharge()) {
        // Can't run and charge.
        return;
    }

    if (closestTarget) {
        auto distance = distanceTo(closestTarget);

        if ((distance <= MAX_CHARGE_DISTANCE) && (distance >= MIN_CHARGE_DISTANCE)) {
            auto chargeDist = (double) rollChargeDistance();
            if (chargeDist >= distance) {
                m_charged = true;

                chargeDist = distance;

                Math::Ray ray(position(), closestTarget->position());
                auto newPos = ray.point_at(chargeDist);
                move(newPos, ray.get_direction());
            } else if (chargeRerolls() != NoRerolls) {
                chargeDist = (double) rollChargeDistance();
                if (chargeDist >= distance) {
                    m_charged = true;

                    chargeDist = distance;

                    Math::Ray ray(position(), closestTarget->position());
                    auto newPos = ray.point_at(chargeDist);
                    move(newPos, ray.get_direction());
                }
            }

            m_currentRecord.m_charged = chargeDist;
        } else {
            // too far to charge
            m_currentRecord.m_charged = 0;
        }
    } else {
        // no target units - stand here confused!!!
        m_currentRecord.m_charged = 0;
    }

    if (m_charged)
        onCharged();
}

void Unit::battleshock(PlayerId player) {
    timeoutBuffs(Phase::Battleshock, player);
}

int Unit::rollChargeDistance() const {
    // TODO: implement rerolls for charges.

    m_unmodifiedChargeRoll = Dice::roll2D6();
    return m_unmodifiedChargeRoll + chargeModifier();
}

int Unit::rollRunDistance() const {
    // TODO: implement rerolls for run.

    return Dice::rollD6() + runModifier();
}

int Unit::slay(int numModels) {
    if (numModels <= 0)
        return 0;

    int numSlain = 0;
    for (auto &model : m_models) {
        if (model->slain() || model->fled()) continue;

        model->slay();
        numSlain++;

        if (numSlain > numModels) break;
    }
    return numSlain;
}

int Unit::heal(int numWounds) {
    if (numWounds <= 0)
        return 0;

    int numHealedWounds = 0;
    for (auto &model : m_models) {
        if (model->slain() || model->fled()) continue;

        auto toHeal = wounds() - model->woundsRemaining();
        model->applyHealing(toHeal);
        if (toHeal >= numWounds) {
            numHealedWounds += numWounds;
            break;
        } else {
            numWounds -= toHeal;
            numHealedWounds += toHeal;
        }
    }
    return numHealedWounds;
}

bool Unit::makeSave(int woundRoll, const Weapon *weapon, int weaponRend, Unit *target, int &saveRoll) {
    auto saveModifiers = toSaveModifier(weapon) + targetSaveModifier(weapon, target);
    auto effectiveRend = m_ignoreRend ? 0 : weaponRend;
    auto toSave = m_save + saveModifiers - effectiveRend;

    saveRoll = Dice::rollD6();
    if (saveRoll < toSave) {
        auto reroll = toSaveRerolls(weapon);
        if (reroll == RerollFailed) {
            saveRoll = Dice::rollD6();
        }
        if ((reroll == RerollOnes) && (woundRoll == 1)) {
            saveRoll = Dice::rollD6();
        }
        if ((reroll == RerollOnesAndTwos) && (woundRoll == 1 || woundRoll == 2)) {
            saveRoll = Dice::rollD6();
        }
    }

    const bool saveMade = (saveRoll >= toSave);

    if (saveMade)
        m_currentRecord.m_savesMade++;
    else
        m_currentRecord.m_savesFailed++;

    return saveMade;
}

void Unit::attackWithWeapon(const Weapon *weapon, Unit *target, const Model *fromModel,
                            Wounds &totalWoundsInflicted, Wounds &totalWoundsSuffered) {
    totalWoundsInflicted = {0, 0, weapon->isMissile() ? Wounds::Source::WeaponMissile : Wounds::Source::WeaponMelee};
    totalWoundsSuffered = {0, 0};

    if (!weapon->isActive()) {
        // not active
        return;
    }

    // check range to target unit
    double distanceToTarget = distanceBetween(fromModel, target);
    if (distanceToTarget > (double) weapon->range()) {
        // out of range
        return;
    }

    const auto rerolls = toHitRerolls(weapon, target);
    const auto woundRerolls = toWoundRerolls(weapon, target);
    const int totalHitModifiers = toHitModifier(weapon, target) + target->targetHitModifier(weapon, this);
    const int totalWoundModifiers = toWoundModifier(weapon, target) + target->targetWoundModifier(weapon, this);

    const int totalAttacks = weapon->numAttacks(extraAttacks(fromModel, weapon, target));
    for (auto a = 0; a < totalAttacks; a++) {
        m_currentRecord.m_attacksMade++;

        // roll to hit
        auto hitRoll = Dice::rollD6();
        auto modifiedHitRoll = hitRoll + totalHitModifiers;
        // TODO: clarify re-rolling - after or before modifiers?
        if (modifiedHitRoll < weapon->toHit()) {
            hitRoll = rerolling(hitRoll, rerolls);
            modifiedHitRoll = hitRoll + totalHitModifiers;
        }

        if (modifiedHitRoll >= weapon->toHit()) {
            // apply hit modifiers (a single hit may result in multiple hits)
            int numHits = generateHits(hitRoll, weapon, target);
            for (auto h = 0; h < numHits; h++) {
                // roll to wound
                auto woundRoll = Dice::rollD6();
                auto modifiedWoundRoll = woundRoll + totalWoundModifiers;
                if (modifiedWoundRoll < weapon->toWound()) {
                    woundRoll = rerolling(woundRoll, woundRerolls);
                    modifiedWoundRoll = woundRoll + totalWoundModifiers;
                }

                if (modifiedWoundRoll >= weapon->toWound()) {
                    m_currentRecord.m_attacksHitting++;

                    // roll save
                    int saveRoll = 0;
                    if (!target->makeSave(woundRoll, weapon, weaponRend(weapon, target, hitRoll, woundRoll), target,
                                          saveRoll)) {
                        // compute damage
                        auto dam = weaponDamage(weapon, target, hitRoll, woundRoll);

                        // modify damage
                        dam = target->targetAttackDamageModifier(dam, this, hitRoll, woundRoll);

                        SimLog(Verbosity::Narrative, "Weapon, %s, inflicted wounds (%d, %d) on %s\n",
                               weapon->name().c_str(), dam.normal, dam.mortal, target->name().c_str());

                        totalWoundsInflicted += dam;
                    } else {
                        // made save
                        SimLog(Verbosity::Narrative, "%s made a save again weapon %s rolling a %d.\n",
                               target->name().c_str(), weapon->name().c_str(), saveRoll);
                    }

                    totalWoundsSuffered += target->computeReturnedDamage(weapon, saveRoll);
                } else {
                    // failed to wound
                    SimLog(Verbosity::Narrative, "Weapon, %s, failed to wound rolling a %d.\n", weapon->name().c_str(),
                           modifiedWoundRoll);
                }
            }
        } else {
            // missed
            SimLog(Verbosity::Narrative, "Weapon, %s, missed with a roll of %d.\n", weapon->name().c_str(),
                   modifiedHitRoll);
        }
    }
}

int Unit::rerolling(int initialRoll, Rerolls reroll) const {
    int roll = initialRoll;
    if (reroll == RerollFailed) {
        roll = Dice::rollD6();
    }
    if ((reroll == RerollOnes) && (initialRoll == 1)) {
        roll = Dice::rollD6();
    }
    if ((reroll == RerollOnesAndTwos) && ((initialRoll == 1) || (initialRoll == 2))) {
        roll = Dice::rollD6();
    }
    return roll;
}

void Unit::computeBattleshockEffect(int roll, int &numFled, int &numRestored) const {
    numFled = (m_modelsSlain + roll) - (m_bravery + braveryModifier());
    numFled = std::max(0, std::min(remainingModels(), numFled));
    numRestored = 0;
}

int Unit::remainingPoints() const {
    auto pointsPerWound = (double) m_points / (double) (m_models.size() * wounds());
    auto points = (int) round(pointsPerWound * (double) remainingWounds());
    return points;
}

bool Unit::unbind(Unit *caster, int castRoll) {
    bool unbound = false;
    if (m_spellsUnbound < m_totalUnbinds) {
        int unbindRoll = Dice::roll2D6() + unbindingModifier();
        if (unbindRoll > castRoll) {
            SimLog(Verbosity::Narrative, "%s unbound a spell cast by %s (%d) with a unbind roll of %d.\n",
                   name().c_str(), caster->name().c_str(), castRoll, unbindRoll);
            unbound = true;
            onUnboundSpell(caster, castRoll);
        } else {
            SimLog(Verbosity::Narrative, "%s failed to unbind a spell cast by %s (%d) with a unbind roll of %d.\n",
                   name().c_str(), caster->name().c_str(), castRoll, unbindRoll);
        }
        m_spellsUnbound++;
    }
    return unbound;
}

void Unit::castSpell() {
    if (m_spellsCast < m_totalSpells) {
        // TODO: Allow unit to cast multiple spells.  A given spell can only be cast once per player per hero phase.

        for (auto &sip : m_knownSpells) {
            if (sip == nullptr) continue;

            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), sip->range());
            if (sip->targetFriendly()) {
                units = Board::Instance()->getUnitsWithin(this, owningPlayer(), sip->range());
                units.push_back(this);
            }

            // TODO: determine 'best' spell to cast on the 'best' target.

            // cast the first spell on the first unit
            for (auto ip : units) {
                bool successful = sip->cast(ip, m_battleRound);
                if (successful) {
                    SimLog(Verbosity::Narrative, "%s successfully cast %s\n", m_name.c_str(), sip->name().c_str());
                    onCastSpell(sip.get(), ip);
                }
                m_spellsCast++;
                return;
            }
        }
    }
}

void Unit::useCommandAbility() {
    int cpAvailable = getCommandPoints();
    if (cpAvailable) {
        // Select _best_ command ability to use (if any)
        AbilityTarget target;
        CommandAbility *cip = SelectCommandAbility(this, target);
        if (cip) {
            if (target.targetUnit)
                cip->apply(target.target);
        }
    }
}

void Unit::makePrayer() {
    if (m_prayersAttempted < m_totalPrayers) {
        // TODO: Allow unit to attempt multiple prayers.  A given prayer can only be attempted once per player per hero phase.
        for (auto &sip : m_knownPrayers) {
            if (sip == nullptr) continue;

            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), sip->range());
            if (sip->targetFriendly()) {
                units = Board::Instance()->getUnitsWithin(this, owningPlayer(), sip->range());
                units.push_back(this);
            }

            // TODO: determine 'best' prayer to attempt on the 'best' target.

            // attempt the first prayer on the first unit
            for (auto ip : units) {
                bool successful = sip->pray(ip, m_battleRound);
                if (successful) {
                    SimLog(Verbosity::Narrative, "%s successfully attempted %s\n", m_name.c_str(), sip->name().c_str());
                }
                m_prayersAttempted++;
                return;
            }
        }
    }
}

bool Unit::buffModifier(BuffableAttribute which, int modifier, Duration duration) {
    ModifierBuff buff = {modifier, duration};
    m_attributeModifiers[which].push_back(buff);

    return true;
}

bool Unit::buffReroll(BuffableAttribute which, Rerolls reroll, Duration duration) {
    RerollBuff buff = {reroll, duration};
    m_rollModifiers[which].push_back(buff);

    return true;
}

bool Unit::buffMovement(MovementRules which, bool allowed, Duration duration) {
    MovementRuleBuff buff = {allowed, duration};
    m_movementRules[which].push_back(buff);

    return true;
}

const Model *Unit::nearestModel(const Model *model, const Unit *targetUnit) const {
    if (!targetUnit || targetUnit->m_models.empty()) { return nullptr; }

    auto nearestModel = targetUnit->m_models[0].get();
    auto minDistance = Math::INFINITE;
    for (const auto &m : targetUnit->m_models) {
        const auto dist = Model::distanceBetween(m.get(), model);
        if (dist < minDistance) {
            minDistance = dist;
            nearestModel = m.get();
        }
    }
    return nearestModel;
}

void Unit::doPileIn() {
    if (!m_meleeTarget) return;

    // Pile in up to 3" towards nearest enemy model.
    for (auto &m : m_models) {
        // Find closest model in melee target unit.
        auto closestTarget = nearestModel(m.get(), m_meleeTarget);
        // Move toward that model if possible
        if (closestTarget) {
            auto totalMoveDistance = std::min((double) m_pileInMove, Model::distanceBetween(m.get(), closestTarget));
            const Math::Ray ray(m->position(), closestTarget->position());
            auto newPos = ray.point_at(totalMoveDistance);
            m->setPosition(newPos);
            //Board::Instance()->moveModel(*m, newPos);
        }
    }
}

int Unit::numModelsWithin(const Model *model, double range) const {
    int count = 0;
    // Count the number of remaining models from this unit within 'range' of the given model.
    for (auto &m : m_models) {
        if (!m->slain() && !m->fled()) {
            double distanceToModel = Model::distanceBetween(m.get(), model);
            if (distanceToModel <= range) count++;
        }
    }
    return count;
}


void Unit::timeoutBuffs(Phase phase, PlayerId player) {
    Duration currentPhase = {phase, m_battleRound, player};

    // Remove all buffs that expire in the player's given phase
    for (auto &list : m_attributeModifiers) {
        for (auto bi = list.begin(); bi != list.end();) {
            if (Expired(bi->duration, currentPhase)) {
                bi = list.erase(bi);
            } else {
                ++bi;
            }
        }
    }

    for (auto &list : m_rollModifiers) {
        for (auto bi = list.begin(); bi != list.end();) {
            if (Expired(bi->duration, currentPhase)) {
                bi = list.erase(bi);
            } else {
                ++bi;
            }
        }
    }

    for (auto &list : m_movementRules) {
        for (auto bi = list.begin(); bi != list.end();) {
            if (Expired(bi->duration, currentPhase)) {
                bi = list.erase(bi);
            } else {
                ++bi;
            }
        }
    }
}

int Unit::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
    int extra = s_globalAttackMod(this, attackingModel, weapon, target, accumulate);

    BuffableAttribute which = AttacksMelee;
    if (weapon->isMissile())
        which = AttacksMissile;

    for (auto bi : m_attributeModifiers[which]) {
        extra += bi.modifier;
    }
    return extra;
}

int Unit::toHitModifier(const Weapon *weapon, const Unit *target) const {
    int modifier = s_globalToHitMod(this, weapon, target, accumulate);

    BuffableAttribute which = ToHitMelee;
    if (weapon->isMissile())
        which = ToHitMissile;

    for (auto bi : m_attributeModifiers[which]) {
        modifier += bi.modifier;
    }
    return modifier;
}

int Unit::toWoundModifier(const Weapon *weapon, const Unit *target) const {
    int modifier = s_globalToWoundMod(this, weapon, target, accumulate);

    BuffableAttribute which = ToWoundMelee;
    if (weapon->isMissile())
        which = ToWoundMissile;

    for (auto bi : m_attributeModifiers[which]) {
        modifier += bi.modifier;
    }
    return modifier;
}

Rerolls Unit::toHitRerolls(const Weapon *weapon, const Unit *target) const {
    BuffableAttribute which = ToHitMelee;
    if (weapon->isMissile())
        which = ToHitMissile;

    auto globalRR = s_globalToHitReroll(this, weapon, target);
    if (globalRR != NoRerolls)
        return globalRR;

    if (m_rollModifiers[which].empty())
        return NoRerolls;
    return m_rollModifiers[which].front().rerolls;
}

Rerolls Unit::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
    BuffableAttribute which = ToWoundMelee;
    if (weapon->isMissile())
        which = ToWoundMissile;

    auto globalRR = s_globalToWoundReroll(this, weapon, target);
    if (globalRR != NoRerolls)
        return globalRR;

    if (m_rollModifiers[which].empty())
        return NoRerolls;
    return m_rollModifiers[which].front().rerolls;
}

int Unit::toSaveModifier(const Weapon *weapon) const {
    int modifier = s_globalSaveMod(this, weapon, accumulate);
    for (auto bi : m_attributeModifiers[ToSave]) {
        modifier += bi.modifier;
    }
    return modifier;
}

Rerolls Unit::toSaveRerolls(const Weapon *weapon) const {
    auto globalRR = s_globalSaveReroll(this, weapon, this);
    if (globalRR != NoRerolls)
        return globalRR;

    if (m_rollModifiers[ToSave].empty())
        return NoRerolls;
    return m_rollModifiers[ToSave].front().rerolls;
}

Rerolls Unit::battleshockRerolls() const {
    auto globalRR = s_globalBattleshockReroll(this);
    if (globalRR != NoRerolls)
        return globalRR;

    if (m_rollModifiers[Bravery].empty())
        return NoRerolls;
    return m_rollModifiers[Bravery].front().rerolls;
}

int Unit::castingModifier() const {
    int modifier = s_globalCastMod(this, accumulate);
    for (auto bi : m_attributeModifiers[CastingRoll]) {
        modifier += bi.modifier;
    }
    return modifier;
}

int Unit::unbindingModifier() const {
    int modifier = s_globalCastMod(this, accumulate);
    for (auto bi : m_attributeModifiers[UnbindingRoll]) {
        modifier += bi.modifier;
    }
    return modifier;
}

int Unit::moveModifier() const {
    int modifier = s_globalMoveMod(this, accumulate);
    for (auto bi : m_attributeModifiers[MoveDistance]) {
        modifier += bi.modifier;
    }
    return modifier;
}

int Unit::runModifier() const {
    int modifier = s_globalRunMod(this, accumulate);
    for (auto bi : m_attributeModifiers[RunDistance]) {
        modifier += bi.modifier;
    }
    return modifier;
}

int Unit::chargeModifier() const {
    int modifier = s_globalChargeMod(this, accumulate);
    for (auto bi : m_attributeModifiers[ChargeDistance]) {
        modifier += bi.modifier;
    }
    return modifier;
}

Rerolls Unit::runRerolls() const {
    auto globalRR = s_globalRunReroll(this);
    if (globalRR != NoRerolls)
        return globalRR;

    if (m_rollModifiers[RunDistance].empty())
        return NoRerolls;
    return m_rollModifiers[RunDistance].front().rerolls;
}

Rerolls Unit::chargeRerolls() const {
    auto globalRR = s_globalChargeReroll(this);
    if (globalRR != NoRerolls)
        return globalRR;

    if (m_rollModifiers[ChargeDistance].empty())
        return NoRerolls;
    return m_rollModifiers[ChargeDistance].front().rerolls;
}

bool Unit::canFly() const {
    if (m_movementRules[Fly].empty())
        return m_fly;
    else
        return m_movementRules[Fly].front().allowed;
}

bool Unit::canRunAndShoot() const {
    if (m_movementRules[RunAndShoot].empty())
        return m_runAndShoot;
    else
        return m_movementRules[RunAndShoot].front().allowed;
}

bool Unit::canRunAndCharge() const {
    if (m_movementRules[RunAndCharge].empty())
        return m_runAndCharge;
    else
        return m_movementRules[RunAndCharge].front().allowed;
}

bool Unit::canRetreatAndShoot() const {
    if (m_movementRules[RetreatAndShoot].empty())
        return m_retreatAndShoot;
    else
        return m_movementRules[RetreatAndShoot].front().allowed;
}

bool Unit::canRetreatAndCharge() const {
    if (m_movementRules[RetreatAndCharge].empty())
        return m_retreatAndCharge;
    else
        return m_movementRules[RetreatAndCharge].front().allowed;
}

int Unit::targetHitModifier(const Weapon *weapon, const Unit * /*attacker*/) const {
    int modifier = 0;

    BuffableAttribute which = TargetToHitMelee;
    if (weapon->isMissile())
        which = TargetToHitMissile;

    for (auto bi : m_attributeModifiers[which]) {
        modifier += bi.modifier;
    }
    return modifier;
}

int Unit::targetWoundModifier(const Weapon *weapon, const Unit * /*attacker*/) const {
    int modifier = 0;

    BuffableAttribute which = TargetToWoundMelee;
    if (weapon->isMissile())
        which = TargetToWoundMissile;

    for (auto bi : m_attributeModifiers[which]) {
        modifier += bi.modifier;
    }
    return modifier;
}

int Unit::targetSaveModifier(const Weapon * /*weapon*/, const Unit * /*attacker*/) const {
    int modifier = 0;

    for (auto bi : m_attributeModifiers[TargetToSave]) {
        modifier += bi.modifier;
    }
    return modifier;
}

int Unit::numOfWoundedModels() const {
    int count = 0;
    for (const auto &m : m_models) {
        if (!m->slain() && !m->fled()) {
            if (m->woundsRemaining() < m->initialWounds()) {
                count++;
            }
        }
    }
    return count;
}

void Unit::visitWeapons(std::function<void(const Weapon &)> &visitor) {
    // Every unit must have at least one weapon.
    assert(!m_weapons.empty());

    for (const auto &w : m_weapons) {
        visitor(*w);
    }
}

int Unit::rollCasting() const {
    return Dice::roll2D6() + castingModifier();
}

PlayerId Unit::owningPlayer() const {
    if (m_roster)
        return m_roster->getOwningPlayer();
    return None;
}

int Unit::getCommandPoints() const {
    if (m_roster)
        return m_roster->getCommandPoints();
    return 0;
}

void Unit::setRoster(Roster *roster) {
    m_roster = roster;
}

int Unit::getModelsWithin(const Model *model, const Unit *targetUnit, double distance) const {
    if (!targetUnit || targetUnit->m_models.empty()) { return 0; }

    int count = 0;
    for (const auto &m : targetUnit->m_models) {
        if (m->slain() || m->fled()) continue;

        const auto dist = Model::distanceBetween(m.get(), model);
        if (dist < distance) {
            count++;
        }
    }
    return count;
}

int Unit::initialWounds() const {
    int wounds = 0;
    for (const auto &m : m_models) {
        wounds += m->initialWounds();
    }
    return wounds;
}

bool Unit::isFriendly(const Unit *unit) const {
    return (unit && (owningPlayer() == unit->owningPlayer()));
}

int Unit::returnModels(int numModels) {
    int numReturned = 0;

    for (auto &m : m_models) {
        if (m->slain()) {
            m->restore();
            numReturned++;
        }
        if (numReturned >= numModels) break;
    }
    return numReturned;
}

int Unit::computeFormation() const {
    // Select the formatation based on number of models, base size and weapon range
    double avgWeaponRange = 0;
    for (auto i = 0; i < numModels(); i++) {
        avgWeaponRange += getModel(i)->preferredWeapon() ? getModel(i)->preferredWeapon()->range() : 0;
    }
    avgWeaponRange /= numModels();
    auto weaponRanks = std::min(4, (int)ceil(avgWeaponRange / basesizeInches()));

    int modelsPerRank = 10;
    if (basesizeInches() <= 30) modelsPerRank = 20;
    else if (basesizeInches() >= 50) modelsPerRank = 1;
    auto numRanks = numModels() / modelsPerRank;

    return std::max(numRanks, weaponRanks);
}

CustomUnit::CustomUnit(const std::string &name, int move, int wounds, int bravery, int save,
                       bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {
}
