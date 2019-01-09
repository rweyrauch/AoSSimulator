/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Roster.h>

void Roster::doHeroPhase()
{
    for (auto u : m_units)
    {
        u->hero();
    }
}

void Roster::doMovementPhase()
{
    for (auto u : m_units)
    {
        u->movement();
    }
}

void Roster::doShootingPhase()
{
    for (auto u : m_units)
    {
        u->shooting();
    }
}

void Roster::doChargePhase()
{
    for (auto u : m_units)
    {
        u->charge();
    }
}

void Roster::doCombatPhase()
{
    for (auto u : m_units)
    {
        u->combat();
    }
}

void Roster::doBattleshockPhase()
{
    for (auto u : m_units)
    {
        u->battleshock();
    }
}

void Roster::beginTurn()
{
    for (auto u : m_units)
    {
        u->beginTurn();
    }
}
