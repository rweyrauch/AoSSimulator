/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cfloat>
#include <Roster.h>

void Roster::doHeroPhase()
{
    for (auto u : m_units)
    {
        u->hero(m_id);
    }
}

void Roster::doMovementPhase()
{
    for (auto u : m_units)
    {
        u->movement(m_id);
    }
}

void Roster::doShootingPhase()
{
    for (auto u : m_units)
    {
        u->shooting(m_id);
    }
}

void Roster::doChargePhase()
{
    for (auto u : m_units)
    {
        u->charge(m_id);
    }
}

void Roster::doCombatPhase()
{
    for (auto u : m_units)
    {
        u->combat(m_id);
    }
}

void Roster::doBattleshockPhase()
{
    for (auto u : m_units)
    {
        u->battleshock(m_id);
    }
}

void Roster::beginTurn(int battleRound, PlayerId playerWithTurn)
{
    for (auto u : m_units)
    {
        u->beginTurn(battleRound, playerWithTurn);
    }
}

Unit *Roster::nearestUnit(const Unit *unit) const
{
    auto nearestUnit = m_units.front();
    float minDistance = FLT_MAX;
    for (auto u : m_units)
    {
        float dist = unit->distanceTo(u);
        if (dist < minDistance)
        {
            minDistance = dist;
            nearestUnit = u;
        }
    }
    return nearestUnit;
}

int Roster::totalPoints() const
{
    int points = 0;
    for (auto u : m_units)
    {
        points += u->points();
    }
    return points;
}

void Roster::endTurn(int battleRound)
{
    for (auto u : m_units)
    {
        u->endTurn(battleRound);
    }
}

void Roster::beginRound(int battleRound)
{
    for (auto u : m_units)
    {
        u->beginRound(battleRound);
    }
}

void Roster::endRound(int battleRound)
{
    for (auto u : m_units)
    {
        u->endRound(battleRound);
    }
}

bool Roster::useCommandPoint()
{
    int cp = getCommandPoints();
    if (cp > 0)
    {
        setCommandPoints(cp-1);
        return true;
    }
    return false;
}

Roster::~Roster()
{
    for (auto ip : m_units)
    {
        delete ip;
    }
}
