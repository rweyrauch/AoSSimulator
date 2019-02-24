/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <iostream>
#include <algorithm>
#include <Unit.h>
#include <Dice.h>
#include <Board.h>
#include <Roster.h>

const float PILE_IN_DISTANCE = 3.0f;
const float MAX_CHARGE_DISTANCE = 12.0f;

Wounds Unit::shoot(int numAttackingModels, Unit* targetUnit, int& numSlain)
{
    if (m_ran && !m_runAndShoot)
    {
        return {0, 0};
    }

    if ((numAttackingModels == -1) || (numAttackingModels > m_models.size()))
    {
        numAttackingModels = (int)m_models.size();
    }

    Wounds totalDamage = {0, 0};
    Wounds totalDamageReflected = {0, 0};

    for (auto i = 0; i < numAttackingModels; i++)
    {
        const Model& model = m_models.at(i);
        if (model.fled() || model.slain()) continue;

        for (auto wip = model.meleeWeaponBegin(); wip != model.meleeWeaponEnd(); ++wip)
        {
            const Weapon *w = *wip;

            Wounds weaponDamage, reflectedDamage;
            attackWithWeapon(w, targetUnit, model, weaponDamage, reflectedDamage);

            // apply wound/mortal wound save
            Wounds damage = targetUnit->applyWoundSave(weaponDamage);

            totalDamage += damage;
            totalDamageReflected += reflectedDamage;
        }
    }

    numSlain = targetUnit->applyDamage(totalDamage);

    if (targetUnit->remainingModels() == 0)
    {
        targetUnit->onSlain();
    }

    // apply returned damage to this unit
    int numSlainByReturnedDamage = applyDamage(totalDamageReflected);
    if (remainingModels() == 0)
    {
        onSlain();
    }

    m_currentRecord.m_woundsInflicted += totalDamage;
    m_currentRecord.m_woundsTaken += totalDamageReflected;
    m_currentRecord.m_enemyModelsSlain += numSlain;
    m_currentRecord.m_modelsSlain += numSlainByReturnedDamage;

    return totalDamage;
}


Wounds Unit::fight(int numAttackingModels, Unit *targetUnit, int &numSlain)
{
    if ((numAttackingModels == -1) || (numAttackingModels > m_models.size()))
    {
        numAttackingModels = (int)m_models.size();
    }

    Wounds totalDamage = {0, 0};
    Wounds totalDamageReflected = {0, 0};

    for (auto i = 0; i < numAttackingModels; i++)
    {
        const Model& model = m_models.at(i);
        if (model.fled() || model.slain()) continue;

        for (auto wip = model.meleeWeaponBegin(); wip != model.meleeWeaponEnd(); ++wip)
        {
            const Weapon *w = *wip;

            Wounds weaponDamage, reflectedDamage;
            attackWithWeapon(w, targetUnit, model, weaponDamage, reflectedDamage);

            // apply wound/mortal wound save
            Wounds damage = targetUnit->applyWoundSave(weaponDamage);

            totalDamage += damage;
            totalDamageReflected += reflectedDamage;
        }
    }

    // Some units do mortal wounds for just existing!  See Evocators for example.
    int mortalWounds = generateMortalWounds(targetUnit);
    totalDamage.mortal += mortalWounds;

    numSlain = targetUnit->applyDamage(totalDamage);

    int numSlainByReturnedDamage = applyDamage(totalDamageReflected);
    if (remainingModels() == 0)
    {
        onSlain();
    }

    m_currentRecord.m_woundsInflicted += totalDamage;
    m_currentRecord.m_woundsTaken += totalDamageReflected;
    m_currentRecord.m_enemyModelsSlain += numSlain;
    m_currentRecord.m_modelsSlain += numSlainByReturnedDamage;

    return totalDamage;
}

int Unit::applyBattleshock()
{
    if (!battleshockRequired()) return 0;
    if (m_modelsSlain <= 0) return 0;

    Dice dice;
    auto roll = rollBattleshock();
    int numFled, numRestored;
    computeBattleshockEffect(roll, numFled, numRestored);

    // mark fleeing models
    int numFleeing = numFled;

    for (auto ip = m_models.rbegin(); ip != m_models.rend() && numFleeing > 0; ++ip)
    {
        if (ip->fled() || ip->slain()) continue;
        ip->flee();
        numFleeing--;
    }

    if (numFled > 0)
        onFlee(numFled);

    m_currentRecord.m_numFled += numFled;

    // restore models
    if (numRestored > 0)
    {
        restoreModels(numRestored);
    }

    return numFled;
}

Unit::Unit(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
    m_name(name),
    m_move(move),
    m_wounds(wounds),
    m_bravery(bravery),
    m_save(save),
    m_fly(fly),
    m_modelsSlain(0)
{
}

void Unit::beginTurn(int battleRound, PlayerId playerWithTurn)
{
    m_battleRound = battleRound;
    m_ran = false;
    m_charged = false;
    m_moved = false;
    m_modelsSlain = 0;

    m_currentRecord.clear();

    m_currentRecord.m_playerWithTurn = playerWithTurn;
    m_currentRecord.m_round = m_battleRound;

    onBeginTurn(battleRound);
}

void Unit::endTurn(int battleRound)
{
    m_statistics.record(m_currentRecord);
}

void Unit::addModel(const Model &model)
{
    m_models.push_back(model);
    m_basesize_mm = model.basesize();
}

int Unit::applyDamage(const Wounds& totalWounds)
{
    // apply both normal and mortal wounds
    int totalDamage = totalWounds.normal + totalWounds.mortal;
    int numSlain = 0;
    for (auto &model : m_models)
    {
        if (model.slain() || model.fled()) continue;

        auto wounds = model.woundsRemaining();
        if (totalDamage >= wounds)
        {
            model.woundsRemaining() = 0;
            model.slay();

            totalDamage -= wounds;
            numSlain++;
        }
        else
        {
            model.woundsRemaining() -= totalDamage;
            totalDamage = 0;
        }
    }

    onWounded();
    m_modelsSlain += numSlain;

    m_currentRecord.m_modelsSlain += numSlain;
    m_currentRecord.m_woundsTaken += totalWounds;

    if (remainingModels() == 0)
    {
        onSlain();
    }

    return numSlain;
}

int Unit::remainingModels() const
{
    int models = 0;
    for (const auto& m : m_models)
    {
        if (!m.slain() && !m.fled())
            models++;
    }
    return models;
}

int Unit::remainingWounds() const
{
    int wounds = 0;
    for (const auto& m : m_models)
    {
        if (!m.slain() && !m.fled())
            wounds += m.woundsRemaining();
    }
    return wounds;
}

bool Unit::hasKeyword(Keyword word) const
{
    auto kw = std::find(m_keywords.begin(), m_keywords.end(), word);
    return (kw != m_keywords.end());
}

bool Unit::addKeyword(Keyword word)
{
    m_keywords.push_back(word);
    return true;
}

int Unit::battlshockModifier() const
{
    return remainingModels() / 10;
}

void Unit::restore()
{
    for (auto& m : m_models)
    {
        m.woundsRemaining() = m_wounds;
        m.restore();
    }

    m_currentRecord.clear();
    m_statistics.reset();

    onRestore();
}

bool Unit::setPosition(const Math::Point3& pos, const Math::Vector3& orientation)
{
    // TODO: pack models into block of (numModels x m_ranks)
    m_position = pos;
    for (auto& m : m_models)
    {
        m.setPosition(pos);
    }

    /*
    int unitW = (int)m_models.size() / m_ranks;
    float unitWidth = m_models.size() * basesizeInches() / (float)m_ranks;

    Math::Vector3 up(0, 0, 1);
    Math::Vector3 left = Math::Vector3::Cross(up, orientation);
    left.normalize();

    int xi = 0;
    int yi = 0;

    Math::Vector3 dx = -left * basesizeInches();
    Math::Vector3 dy = -orientation * basesizeInches();

    Math::Vector3 pos0 = Math::Vector3(pos.x(), pos.y(), pos.z()) + left * unitWidth / 2.0f;
    Math::Vector3 curPos = pos0;

    for (auto& m : m_models)
    {
        m.setPosition(Math::Point3(curPos.x(), curPos.y(), curPos.z()));
        m.setPosition(pos);

        xi++;
        curPos += dx;
        if (xi > unitW)
        {
            xi = 0;
            yi++;

            curPos = pos0;
            curPos += (float)yi * dy;
        }
    }
    */
    return true;
}

bool Unit::formation(int ranks)
{
    if (ranks < 1 || ranks > m_models.size())
        return false;

    m_ranks = ranks;
    return true;
}

float Unit::distanceTo(const Unit* unit) const
{
    if (unit == nullptr || unit->remainingModels() == 0 || remainingModels() == 0)
        return 0.0f;

    float dist = position().distance(unit->position());
    return std::max(0.0f, dist - (basesizeInches() + unit->basesizeInches())/2.0f);
}

float Unit::distanceBetween(const Model* model, const Unit* unit) const
{
    if (model == nullptr || unit == nullptr || unit->remainingModels() == 0)
        return 0.0f;

    // TODO: find closes model in target unit
    const float tx = unit->m_models.front().x();
    const float ty = unit->m_models.front().y();
    const float tbs = unit->basesizeInches() / 2;

    const float x = model->x();
    const float y = model->y();

    const float dx = fabsf(tx - x);
    const float dy = fabsf(ty - y);

    return std::max(0.0f, sqrtf(dx*dx + dy*dy) - (basesizeInches() / 2 + tbs));
}

void Unit::movement(PlayerId player)
{
    auto board = Board::Instance();

    m_moved = false;

    // Unit cannot move
    if (!m_canMove) return;

    auto weapon = m_models.front().preferredWeapon();
    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);
    auto closestTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    if (closestTarget)
    {
        Dice dice;
        auto distance = distanceTo(closestTarget);
        float totalMoveDistance = 0.0f;
        if (weapon->isMissile())
        {
            // get into range (run or not?)
            if (distance > weapon->range() + move())
            {
                // too far to get into range with normal move - run
                auto runDist = rollRunDistance();
                if ((runDist < 3) && (runRerolls() != NoRerolls))
                    runDist = rollRunDistance();
                totalMoveDistance = move() + runDist;
                m_ran = true;

                m_currentRecord.m_moved = move();
                m_currentRecord.m_ran = runDist;
            }
            else if (distance > weapon->range())
            {
                // move toward unit
                totalMoveDistance = move();

                m_currentRecord.m_moved = move();
            }
            else
            {
                // already in range - stand still
                totalMoveDistance = 0.0f;

                m_currentRecord.m_moved = 0.0f;
            }
        }
        else
        {
            // get into range (run or not?)
            if (distance > weapon->range() + move() + PILE_IN_DISTANCE) // todo: pile-in should be unit-specific
            {
                // too far to get into range with normal move - run
                auto runDist = rollRunDistance();
                if ((runDist < 3) && (runRerolls() != NoRerolls))
                    runDist = rollRunDistance();
                totalMoveDistance = move() + runDist;
                m_ran = true;

                m_currentRecord.m_moved = move();
                m_currentRecord.m_ran = runDist;
            }
            else if (distance > PILE_IN_DISTANCE) // pile-in
            {
                // move toward unit
                totalMoveDistance = std::min((float)move(), distance - PILE_IN_DISTANCE);

                m_currentRecord.m_moved = totalMoveDistance;
            }
            else
            {
                // already in range - stand still
                totalMoveDistance = 0.0f;

                m_currentRecord.m_moved = 0.0f;
            }
        }
        Math::Ray ray(position(), closestTarget->position());
        auto newPos = ray.point_at(totalMoveDistance);
        setPosition(newPos, ray.get_direction());

        m_moved = (totalMoveDistance > 0.0f);
    }
    else
    {
        // no target units - stand here confused!!!
        m_moved = false;
    }

    if (m_ran)
        onRan();
}

int Unit::rollBattleshock() const
{
    Dice dice;
    return dice.rollD6();
}

Wounds Unit::shoot(int& numSlain)
{
    if (m_shootingTarget == nullptr)
    {
        numSlain = 0;
        return {0, 0};
    }
    return shoot(-1, m_shootingTarget, numSlain);
}

Wounds Unit::fight(PlayerId player, int &numSlain)
{
    if (m_meleeTarget == nullptr)
    {
        numSlain = 0;
        return {0, 0};
    }
    auto wounds = fight(-1, m_meleeTarget, numSlain);

    onEndCombat(player);

    return wounds;
}

void Unit::shooting(PlayerId player)
{
    auto board = Board::Instance();

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    m_shootingTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    onStartShooting(player);
}

void Unit::combat(PlayerId player)
{
    auto board = Board::Instance();

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    m_meleeTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    // TODO: pile-in

    onStartCombat(player);
}

void Unit::charge(PlayerId player)
{
    auto board = Board::Instance();

    auto weapon = m_models.front().preferredWeapon();
    if (weapon->isMissile()) return;

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);
    auto closestTarget = otherRoster ? otherRoster->nearestUnit(this) : nullptr;

    if (m_ran && !m_runAndCharge)
    {
        // Can't run and charge.
        return;
    }

    if (closestTarget)
    {
        Dice dice;
        auto distance = distanceTo(closestTarget);

        if (distance < MAX_CHARGE_DISTANCE && distance > 3.0)
        {
            float chargeDist = rollChargeDistance();
            if (chargeDist >= distance)
            {
                m_charged = true;

                chargeDist = distance;

                Math::Ray ray(position(), closestTarget->position());
                auto newPos = ray.point_at(chargeDist);
                setPosition(newPos, ray.get_direction());
            }
            else if (chargeRerolls() != NoRerolls)
            {
                chargeDist = rollChargeDistance();
                if (chargeDist >= distance)
                {
                    m_charged = true;

                    chargeDist = distance;

                    Math::Ray ray(position(), closestTarget->position());
                    auto newPos = ray.point_at(chargeDist);
                    setPosition(newPos, ray.get_direction());
                }
            }

            m_currentRecord.m_charged = chargeDist;
        }
        else
        {
            // too far to charge
            m_currentRecord.m_charged = 0.0f;
        }
    }
    else
    {
        // no target units - stand here confused!!!
        m_currentRecord.m_charged = 0.0f;
    }

    if (m_charged)
        onCharged();
}

void Unit::battleshock(PlayerId player)
{

}

int Unit::rollChargeDistance() const
{
    Dice dice;
    return dice.roll2D6() + chargeModifier();
}

int Unit::rollRunDistance() const
{
    Dice dice;
    return dice.rollD6() + runModifier();
}

int Unit::slay(int numModels)
{
    int numSlain = 0;
    for (auto &model : m_models)
    {
        if (model.slain() || model.fled()) continue;

        model.woundsRemaining() = 0;
        model.slay();

        numSlain++;

        if (numSlain > numModels) break;
    }
    return numSlain;
}

int Unit::heal(int numWounds)
{
    if (numWounds <= 0)
        return 0;

    int numHealedWounds = 0;
    for (auto &model : m_models)
    {
        if (model.slain() || model.fled()) continue;

        auto toHeal = wounds() - model.woundsRemaining();
        if (toHeal >= numWounds)
        {
            model.woundsRemaining() += numWounds;
            numHealedWounds += numWounds;
            break;
        }
        else
        {
            model.woundsRemaining() += toHeal;
            numWounds -= toHeal;
            numHealedWounds += toHeal;
        }
    }
    return numHealedWounds;
}

bool Unit::makeSave(int woundRoll, const Weapon* weapon, int& saveRoll)
{
    Dice dice;

    auto effectiveRend = m_ignoreRend ? 0 : weapon->rend();
    auto toSave = m_save - effectiveRend;

    saveRoll = dice.rollD6();
    if (saveRoll < toSave)
    {
        auto reroll = toSaveRerolls(weapon);
        if (reroll == RerollFailed)
        {
            saveRoll = dice.rollD6();
        }
        else if (reroll == RerollOnes && woundRoll == 1)
        {
            saveRoll = dice.rollD6();
        }
        else if (reroll == RerollOnesAndTwos && (woundRoll == 1 || woundRoll == 2))
        {
            saveRoll = dice.rollD6();
        }
    }

    const bool saveMade = (saveRoll >= toSave);

    if (saveMade)
        m_currentRecord.m_savesMade++;
    else
        m_currentRecord.m_savesFailed++;

    return saveMade;
}

void Unit::attackWithWeapon(const Weapon* weapon, Unit* target, const Model& fromModel,
    Wounds& totalWoundsInflicted, Wounds& totalWoundsSuffered)
{
    totalWoundsInflicted = {0, 0};
    totalWoundsSuffered = {0, 0};

    if (!weapon->isActive())
    {
        // not active
        return;
    }

    // check range to target unit
    float distanceToTarget = distanceBetween(&fromModel, target);
    if (distanceToTarget > weapon->range())
    {
        // out of range
        return;
    }

    Dice dice;
    const auto rerolls = toHitRerolls(weapon, target);
    const auto woundRerolls = toWoundRerolls(weapon, target);
    const int totalHitModifiers = toHitModifier(weapon, target) + target->targetHitModifier(weapon, this);
    const int totalWoundModifiers = toWoundModifier(weapon, target) + target->targetWoundModifier(weapon, this);

    const int totalAttacks = weapon->numAttacks(extraAttacks(weapon));
    for (auto a = 0; a < totalAttacks; a++)
    {
        m_currentRecord.m_attacksMade++;

        // roll to hit
        auto hitRoll = dice.rollD6();
        auto modifiedHitRoll = hitRoll + totalHitModifiers;
        // TODO: clarify re-rolling - after or before modifiers?
        if (modifiedHitRoll < weapon->toHit())
        {
            hitRoll = rerolling(hitRoll, rerolls, dice);
            modifiedHitRoll = hitRoll + totalHitModifiers;
        }

        if (modifiedHitRoll >= weapon->toHit())
        {
            // apply hit modifiers (a single hit may result in multiple hits)
            int numHits = generateHits(hitRoll, weapon, target);
            for (auto h = 0; h < numHits; h++)
            {
                // roll to wound
                auto woundRoll = dice.rollD6();
                auto modifiedWoundRoll = woundRoll + totalWoundModifiers;
                if (modifiedWoundRoll < weapon->toWound())
                {
                    woundRoll = rerolling(woundRoll, woundRerolls, dice);
                    modifiedWoundRoll = woundRoll + totalWoundModifiers;
                }

                if (modifiedWoundRoll >= weapon->toWound())
                {
                    m_currentRecord.m_attacksHitting++;

                    // roll save
                    int saveRoll = 0;
                    if (!target->makeSave(woundRoll, weapon, saveRoll))
                    {
                        // compute damage
                        totalWoundsInflicted += weaponDamage(weapon, target, hitRoll, woundRoll);
                    }
                    else
                    {
                        // made save
                    }

                    totalWoundsSuffered += target->computeReturnedDamage(weapon, saveRoll);

                }
                else
                {
                    // failed to wound
                }
            }
        }
        else
        {
            // missed
        }
    }
}

int Unit::rerolling(int initialRoll, Rerolls reroll, Dice& dice) const
{
    int roll = initialRoll;
    if (reroll == RerollFailed)
    {
        roll = dice.rollD6();
    }
    else if ((reroll == RerollOnes) && (initialRoll == 1))
    {
        roll = dice.rollD6();
    }
    else if ((reroll == RerollOnesAndTwos) && ((initialRoll == 1) || (initialRoll == 2)))
    {
        roll = dice.rollD6();
    }
    return roll;
}

void Unit::computeBattleshockEffect(int roll, int& numFled, int& numRestored) const
{
    numFled = (m_modelsSlain + roll) - (m_bravery + battlshockModifier());
    numFled = std::max(0, std::min(remainingModels(), numFled));
    numRestored = 0;
}

int Unit::remainingPoints() const
{
    auto pointsPerModel = (float)m_points / m_models.size();
    auto points = (int)roundf(pointsPerModel * remainingModels());
    return points;
}

bool Unit::unbind(const Unit* caster, int castRoll)
{
    return false;
}

CustomUnit::CustomUnit(const std::string &name, int move, int wounds, int bravery, int save,
                       bool fly) :
    Unit(name, move, wounds, bravery, save, fly)
{
}
