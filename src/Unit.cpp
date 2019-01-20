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

            auto damage = unit->computeDamage(numWounds, numMortalWounds, w);
            totalDamage.normal += damage.normal;
            totalDamage.mortal += damage.mortal;
        }
    }

    numSlain = unit->applyDamage(totalDamage);
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

            auto damage = unit->computeDamage(totalWounds, numMortalWounds, w);
            totalDamage.normal += damage.normal;
            totalDamage.mortal += damage.mortal;
        }
    }

    numSlain = unit->applyDamage(totalDamage);
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

    m_modelsSlain = 0;

    return numFled;
}

Wounds Unit::computeDamage(const WoundingHits& woundingHits, int mortalWounds, const Weapon *weapon)
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

void Unit::beginTurn()
{
    m_ran = false;
    m_charged = false;
}

void Unit::addModel(const Model &model)
{
    m_models.push_back(model);
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
    m_modelsSlain = numSlain;
    return m_modelsSlain;
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

bool Unit::position(float x, float y, float z)
{
    // TODO: pack models into block of (numModels x m_ranks)
    for (auto& m : m_models)
    {
        m.position(x, y, z);
    }
    return true;
}

void Unit::getPosition(float& x, float& y, float& z) const
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    if (!m_models.empty())
    {
        auto model = m_models.begin();
        x = model->x();
        y = model->y();
        z = model->z();
    }
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

    // TODO: find closest models in this unit and the target unit
    auto model = m_models.at(0);
    return distanceBetween(&model, unit);
}

float Unit::distanceBetween(const Model* model, const Unit* unit) const
{
    if (model == nullptr || unit == nullptr || unit->remainingModels() == 0)
        return 0.0f;

    // TODO: find closes model in target unit
    const float tx = unit->m_models.begin()->x();
    const float ty = unit->m_models.begin()->y();

    const float x = model->x();
    const float y = model->y();

    const float dx = fabsf(tx - x);
    const float dy = fabsf(ty - y);

    return sqrtf(dx*dx + dy*dy);
}

void Unit::movement(PlayerId player)
{
    auto board = Board::Instance();
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

Wounds Unit::fight(int &numSlain)
{
    if (m_meleeTarget == nullptr)
    {
        numSlain = 0;
        return {0, 0};
    }
    return fight(-1, m_meleeTarget, numSlain);
}

void Unit::shooting(PlayerId player)
{
    auto board = Board::Instance();

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    m_shootingTarget = otherRoster->nearestUnit(this);
}

void Unit::combat(PlayerId player)
{
    auto board = Board::Instance();

    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
        otherPlayer = PlayerId::Blue;
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    m_meleeTarget = otherRoster->nearestUnit(this);
}

CustomUnit::CustomUnit(const std::string &name, int move, int wounds, int bravery, int save,
                       bool fly) :
    Unit(name, move, wounds, bravery, save, fly)
{
}
