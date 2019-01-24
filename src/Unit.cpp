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

Wounds Unit::shoot(int numAttackingModels, Unit* unit, int& numSlain)
{
    if (m_ran && !m_runAndShoot)
    {
        return {0, 0};
    }

    if ((numAttackingModels == -1) || (numAttackingModels > m_models.size()))
    {
        numAttackingModels = (int)m_models.size();
    }

    //std::cout << "Distance between " << name() << " and target " << unit->name() << " is " << distanceTo(unit) << std::endl;

    Wounds totalDamage = {0, 0};
    Wounds totalDamageReturned = {0, 0};

    for (auto i = 0; i < numAttackingModels; i++)
    {
        const Model& model = m_models.at(i);
        if (model.fled() || model.slain()) continue;

        for (auto wip = model.missileWeaponBegin(); wip != model.missileWeaponEnd(); ++wip)
        {
            Weapon* w = *wip;

            if (!w->isActive())
            {
                // not active
                continue;
            }

            // check range to target unit
            float distanceToTarget = distanceBetween(&model, unit);
            if (distanceToTarget > w->range())
            {
                // out of range
                continue;
            }

            auto hits = w->rollToHit(toHitModifier(w, unit), toHitRerolls(w, unit), extraAttacks(w));

            // apply hit modifiers on returned hits
            hits = applyHitModifiers(w, unit, hits);

            auto numWounds = w->rollToWound(hits.numHits, toWoundModifier(w, unit), toWoundRerolls(w, unit));

            int numMortalWounds = generateMortalWounds(w, unit, hits);

            // some units being targeted generate wounds to the attacking units on successful saves
            Wounds damageReturned = {0, 0};
            auto damage = unit->computeDamage(numWounds, numMortalWounds, w, damageReturned);

            totalDamage.normal += damage.normal;
            totalDamage.mortal += damage.mortal;

            totalDamageReturned.normal += damageReturned.normal;
            totalDamageReturned.mortal += damageReturned.mortal;
        }
    }

    numSlain = unit->applyDamage(totalDamage);

    // apply returned damage to this unit
    int numSlainInReturn = applyDamage(totalDamageReturned);
    if (numSlainInReturn)
    {
        std::cout << "Return damage killed " << numSlainInReturn << " models in the attacking unit." << std::endl;
    }

    return totalDamage;
}

Wounds Unit::fight(int numAttackingModels, Unit *unit, int& numSlain)
{
    if ((numAttackingModels == -1) || (numAttackingModels > m_models.size()))
    {
        numAttackingModels = (int)m_models.size();
    }

    //std::cout << "Distance between " << name() << " and target " << unit->name() << " is " << distanceTo(unit) << std::endl;

    Wounds totalDamage = {0, 0};
    Wounds totalDamageReflected = {0, 0};

    for (auto i = 0; i < numAttackingModels; i++)
    {
        const Model& model = m_models.at(i);
        if (model.fled() || model.slain()) continue;

        for (auto wip = model.meleeWeaponBegin(); wip != model.meleeWeaponEnd(); ++wip)
        {
            Weapon* w = *wip;

            if (!w->isActive())
            {
                // not active
                continue;
            }

            // check range to target unit
            float distanceToTarget = distanceBetween(&model, unit);
            if (distanceToTarget > w->range())
            {
                // out of range
                continue;
            }

            auto hits = w->rollToHit(toHitModifier(w, unit), toHitRerolls(w, unit), extraAttacks(w));
            // apply hit modifiers on returned hits
            hits = applyHitModifiers(w, unit, hits);

            auto totalWounds = w->rollToWound(hits.numHits, toWoundModifier(w, unit), toWoundRerolls(w, unit));
            int numMortalWounds = generateMortalWounds(w, unit, hits);

            // some units being targeted generate wounds to the attacking units on successful saves
            Wounds damageReflected = {0, 0};
            auto damage = unit->computeDamage(totalWounds, numMortalWounds, w, damageReflected);
            totalDamage.normal += damage.normal;
            totalDamage.mortal += damage.mortal;

            totalDamageReflected.normal += damageReflected.normal;
            totalDamageReflected.mortal += damageReflected.mortal;
        }
    }

    numSlain = unit->applyDamage(totalDamage);

    int numSlainByReturnedDamage = applyDamage(totalDamageReflected);
    if (numSlainByReturnedDamage)
    {
        std::cout << "Number of attacking models slain by reflected damage: " << numSlainByReturnedDamage
                  << "  Total reflected damage: " << totalDamageReflected.normal << "  Mortal: " << totalDamageReflected.mortal << std::endl;
    }

    return totalDamage;
}

int Unit::applyBattleshock()
{
    if (m_modelsSlain <= 0) return 0;

    Dice dice;
    auto roll = rollBattleshock();
    int numFled = (m_modelsSlain + roll) - (m_bravery + battlshockModifier());
    numFled = std::max(0, std::min(remainingModels(), numFled));

    //std::cout << "Battleshock: Models Slain: " << m_modelsSlain << "  Roll: "
    //    << roll << "  Bravery: " << m_bravery << " modifier: " << battlshockModifier() << std::endl;

    // mark fleeing models
    int numFleeing = numFled;

    for (auto ip = m_models.rbegin(); ip != m_models.rend() && numFleeing > 0; ++ip)
    {
        if (ip->fled() || ip->slain()) continue;
        ip->flee();
        numFleeing--;
    }

    return numFled;
}

Wounds Unit::computeDamage(const WoundingHits& woundingHits, int mortalWounds, const Weapon *weapon,
    Wounds& woundsReturned)
{
    Dice dice;
    Dice::RollResult rollResult;

    auto effectiveRend = m_ignoreRend ? 0 : weapon->rend();
    auto toSave = m_save - effectiveRend;

    int numMadeSaves = 0;
    if (toSaveModifier(weapon) == RerollOnes)
    {
        dice.rollD6(woundingHits.numWoundingHit, 1, rollResult);
        numMadeSaves = rollResult.rollsGE(toSave);
    }
    else if (toSaveModifier(weapon) == RerollFailed)
    {
        dice.rollD6(woundingHits.numWoundingHit, rollResult);
        numMadeSaves = rollResult.rollsGE(toSave);
        int numFails = woundingHits.numWoundingHit - numMadeSaves;
        if (numFails > 0)
        {
            dice.rollD6(numFails, rollResult);
            auto numRerolledSaves = rollResult.rollsGE(toSave);
            numMadeSaves += numRerolledSaves;
        }
    }
    else
    {
        dice.rollD6(woundingHits.numWoundingHit, rollResult);
        numMadeSaves = rollResult.rollsGE(toSave);
    }

    auto numFails = woundingHits.numWoundingHit - numMadeSaves;
    auto totalDamage = numFails * weapon->damage();

    // TODO: add mortal wound save

    // add returned damage (damage inflicted by this unit on the attacking unit)
    woundsReturned = computeReturnedDamage(weapon, rollResult);

    return {totalDamage, mortalWounds};
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

void Unit::beginTurn(int battleRound)
{
    m_battleRound = battleRound;
    m_ran = false;
    m_charged = false;
    m_modelsSlain = 0;

    onBeginTurn(battleRound);
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

    auto weapon = m_models.front().preferredWeapon();
    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);
    auto closestTarget = otherRoster->nearestUnit(this);

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
                totalMoveDistance = move() + rollRunDistance();
                m_ran = true;
            }
            else if (distance > weapon->range())
            {
                // move toward unit
                totalMoveDistance = move();
            }
            else
            {
                // already in range - stand still
                totalMoveDistance = 0.0f;
            }
        }
        else
        {
            // get into range (run or not?)
            if (distance > weapon->range() + move() + PILE_IN_DISTANCE) // todo: pile-in should be unit-specific
            {
                // too far to get into range with normal move - run
                totalMoveDistance = move() + rollRunDistance();
                m_ran = true;
            }
            else if (distance > PILE_IN_DISTANCE) // pile-in
            {
                // move toward unit
                totalMoveDistance = std::min((float)move(), distance - PILE_IN_DISTANCE);
            }
            else
            {
                // already in range - stand still
                totalMoveDistance = 0.0f;
            }
        }
        Math::Ray ray(position(), closestTarget->position());
        auto newPos = ray.point_at(totalMoveDistance);
        setPosition(newPos, ray.get_direction());
    }
    else
    {
        // no target units - stand here confused!!!
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

    m_shootingTarget = otherRoster->nearestUnit(this);

    onStartShooting(player);
}

void Unit::combat(PlayerId player)
{
    auto board = Board::Instance();

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    m_meleeTarget = otherRoster->nearestUnit(this);

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
    auto closestTarget = otherRoster->nearestUnit(this);

    if (closestTarget)
    {
        Dice dice;
        auto distance = distanceTo(closestTarget);

        if (distance < MAX_CHARGE_DISTANCE)
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
        }
    }
    else
    {
        // no target units - stand here confused!!!
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

CustomUnit::CustomUnit(const std::string &name, int move, int wounds, int bravery, int save,
                       bool fly) :
    Unit(name, move, wounds, bravery, save, fly)
{
}
