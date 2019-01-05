/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <Unit.h>

#include "Unit.h"
#include "Dice.h"


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
            auto numHits = w->rollToHit();
            auto numWounds = w->rollToWound(numHits);

            totalDamage += unit.takeDamage(numWounds, *w);
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
        numAttackingModels = m_models.size();
    }

    int totalDamage = 0;
    for (auto i = 0; i < numAttackingModels; i++)
    {
        const Model& model = m_models.at(i);
        for (auto w = model.meleeWeaponBegin(); w != model.meleeWeaponEnd(); ++w)
        {
            auto numHits = w->rollToHit();
            if (numHits > 0)
            {
                auto numWounds = w->rollToWound(numHits);
                if (numWounds > 0)
                {
                    totalDamage += unit.takeDamage(numWounds, *w);
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
    if (m_modelsSlain <= 0) return true;

    Dice dice;
    auto roll = dice.rollD6();
    roll += modifier;
    int numFleeing = (m_modelsSlain + roll) - (m_bravery + modifier);
    numFleeing = std::min((int)m_models.size(), numFleeing);

    // remove fleeing models
    int numFled = numFleeing;
    while (numFled > 0)
    {
        m_models.pop_back();
        --numFled;
    }

    return numFleeing;
}

int Unit::takeDamage(int numWoundingHits, const Weapon& weapon)
{
    Dice dice;
    auto rolls = dice.rollD6(numWoundingHits);
    auto toSave = m_save + weapon.rend();
    auto damageCheck = [toSave](int v) { return v < toSave; };
    auto numHits = std::count_if(rolls.begin(), rolls.end(), damageCheck);
    auto totalDamage = numHits * weapon.damage();
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

