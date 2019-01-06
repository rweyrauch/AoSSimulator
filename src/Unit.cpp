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

void Unit::hero()
{

}

void Unit::movement(bool run)
{
    m_ran = run;
}

void Unit::shooting(int numAttackingModels, Unit &unit)
{
    if (m_ran && !m_runAndShoot)
    {
        return;
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
}

void Unit::charge()
{
    if (m_ran && !m_runAndCharge)
    {
        return;
    }
    m_charged = true;
}

int Unit::combat(int numAttackingModels, Unit &unit)
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

int Unit::battleshock(int modifier)
{
    if (m_modelsSlain <= 0) return 0;

    Dice dice;
    auto roll = dice.rollD6();
    roll += modifier;
    int numFleeing = (m_modelsSlain + roll) - (m_bravery + modifier);
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
    auto toSave = m_save + effectiveRend;

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
    for (auto m = m_models.begin(); m != m_models.end(); ++m)
    {
        auto wounds = m->woundsRemaining();
        if (totalDamage > wounds)
        {
            m->woundsRemaining() = 0;
            totalDamage -= wounds;
        }
        else
        {
            m->woundsRemaining() -= totalDamage;
            totalDamage = 0;
        }
        if (m->woundsRemaining() <= 0)
        {
            m = m_models.erase(m);
            numSlain++;
        }
    }

    m_modelsSlain = numSlain;

    return numSlain;
}

CustomUnit::CustomUnit(const std::string &name, int move, int wounds, int bravery, int save,
                       bool fly) :
    Unit(name, move, wounds, bravery, save, fly)
{
}
