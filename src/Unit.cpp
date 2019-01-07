/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <Unit.h>
#include <Dice.h>

int Unit::shoot(int numAttackingModels, Unit &unit)
{
    if (m_ran && !m_runAndShoot)
    {
        return 0;
    }

    if ((numAttackingModels == -1) || (numAttackingModels > m_models.size()))
    {
        numAttackingModels = m_models.size();
    }

    int totalDamage = 0;
    for (auto i = 0; i < numAttackingModels; i++)
    {
        const Model& model = m_models.at(i);
        for (auto w = model.missileWeaponBegin(); w != model.missileWeaponEnd(); ++w)
        {
            auto numHits = w->rollToHit(toHitModifierMissile(unit), toHitRerollsMissile(unit), extraAttacksMissile(), hitModifierMissile());
            auto numWounds = w->rollToWound(numHits, toWoundModifierMissile(unit), toWoundRerollsMissile(unit));

            totalDamage += unit.computeDamage(numWounds, *w);
        }
    }

    auto slain = unit.applyDamage(totalDamage);
    if (slain)
    {
        // unit was slain
    }
    return totalDamage;
}

int Unit::fight(int numAttackingModels, Unit &unit)
{
    if ((numAttackingModels == -1) || (numAttackingModels > m_models.size()))
    {
        numAttackingModels = (int)m_models.size();
    }

    int totalDamage = 0;
    for (auto i = 0; i < numAttackingModels; i++)
    {
        const Model& model = m_models.at(i);
        for (auto w = model.meleeWeaponBegin(); w != model.meleeWeaponEnd(); ++w)
        {
            auto numHits = w->rollToHit(toHitModifier(unit), toHitRerolls(unit), extraAttacks(), hitModifier());
            if (numHits > 0)
            {
                auto numWounds = w->rollToWound(numHits, toWoundModifier(unit), toWoundRerolls(unit));
                if (numWounds > 0)
                {
                    totalDamage += unit.computeDamage(numWounds, *w);
                }
            }
        }
    }

    auto slain = unit.applyDamage(totalDamage);
    if (slain)
    {
        // unit was slain
    }

    return totalDamage;
}

int Unit::applyBattleshock()
{
    if (m_modelsSlain <= 0) return 0;

    Dice dice;
    auto roll = dice.rollD6();
    int numFleeing = (m_modelsSlain + roll) - (m_bravery + battlshockModifier());
    numFleeing = std::max(0, std::min((int)m_models.size(), numFleeing));

    // remove fleeing models
    int numFled = numFleeing;
    while (numFled > 0)
    {
        m_models.pop_back();
        --numFled;
    }

    return numFleeing;
}

int Unit::computeDamage(int numWoundingHits, const Weapon &weapon)
{
    Dice dice;
    Dice::RollResult rollResult;

    auto effectiveRend = m_ignoreRend ? 0 : weapon.rend();
    auto toSave = m_save - effectiveRend;

    int numMadeSaves = 0;
    if (toSaveModifier() == RerollOnes)
    {
        dice.rollD6(numWoundingHits, 1, rollResult);
        numMadeSaves = rollResult.rollsGE(toSave);
    }
    else if (toSaveModifier() == RerollFailed)
    {
        dice.rollD6(numWoundingHits, rollResult);
        numMadeSaves = rollResult.rollsGE(toSave);
        int numFails = numWoundingHits - numMadeSaves;
        if (numFails > 0)
        {
            dice.rollD6(numFails, rollResult);
            auto numRerolledSaves = rollResult.rollsGE(toSave);
            numMadeSaves += numRerolledSaves;
        }
    }
    else
    {
        dice.rollD6(numWoundingHits, rollResult);
        numMadeSaves = rollResult.rollsGE(toSave);
    }

    auto numFails = numWoundingHits - numMadeSaves;
    auto totalDamage = numFails * weapon.damage();
    return totalDamage;
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
    m_modelsSlain = 0;
    m_ran = false;
}

void Unit::addModel(const Model &model)
{
    m_models.push_back(model);
}

int Unit::applyDamage(int totalDamage)
{
    int numSlain = 0;
    for (auto &model : m_models)
    {
        auto wounds = model.woundsRemaining();
        if (totalDamage > wounds)
        {
            model.woundsRemaining() = 0;
            totalDamage -= wounds;
            numSlain++;
        }
        else
        {
            model.woundsRemaining() -= totalDamage;
            totalDamage = 0;
        }
    }
    m_modelsSlain = numSlain;

    // Remove slain models. Put models with no wounds remaining at the end of the
    // model list.  Then remove them from the end (back) of the vector.
    auto compFunc = [](const Model& a, const Model& b)->bool {
        return a.woundsRemaining() > b.woundsRemaining();
    };
    std::sort(m_models.begin(), m_models.end(), compFunc);
    while (numSlain > 0)
    {
        m_models.pop_back();
        --numSlain;
    }
    return m_modelsSlain;
}

int Unit::remainingModels() const
{
    int models = 0;
    for (auto m : m_models)
    {
        if (m.woundsRemaining() > 0)
            models++;
    }
    return models;
}

int Unit::remainingWounds() const
{
    int wounds = 0;
    for (auto m : m_models)
    {
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

CustomUnit::CustomUnit(const std::string &name, int move, int wounds, int bravery, int save,
                       bool fly) :
    Unit(name, move, wounds, bravery, save, fly)
{
}
