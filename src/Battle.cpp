/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include <Dice.h>
#include <Battle.h>

void Battle::start(PlayerId firstPlayer)
{
    m_topOfRound = true;
    m_currentPlayer = firstPlayer;
    m_currentPhase = Phase::Hero;
    m_round = 1;
}

void Battle::next()
{
    // advance battle state machine
    switch (m_currentPhase)
    {
        case Phase::Initiative:
            m_currentPhase = Phase::Hero;
            break;
        case Phase::Hero:
            m_currentPhase = Phase::Movement;
            break;
        case Phase::Movement:
            m_currentPhase = Phase::Shooting;
            break;
        case Phase::Shooting:
            m_currentPhase = Phase::Charge;
            break;
        case Phase::Charge:
            m_currentPhase = Phase::Combat;
            break;
        case Phase::Combat:
            m_currentPhase = Phase::Battleshock;
            break;
        case Phase::Battleshock:
            if (m_topOfRound)
            {
                // Next players turn
                m_topOfRound = false;
                if (m_currentPlayer == PlayerId::One)
                {
                    m_currentPlayer = PlayerId::Two;
                }
                else
                {
                    m_currentPlayer = PlayerId::One;
                }
                m_currentPhase = Phase::Hero;
            }
            else
            {
                // End of round.
                m_currentPhase = Phase::Initiative;
                m_topOfRound = true;
                m_round++;

                // End of battle.
                if (m_round > m_numRounds)
                {
                    m_isDone = true;
                }
            }
            break;
    }
}

bool Battle::done()
{
    return m_isDone;
}

void Battle::simulate()
{
    std::cout << "Battle State:" << std::endl;
    std::cout << "\tRound: " << m_round << " of " << m_numRounds << ".  Top of round: " << m_topOfRound << std::endl;
    std::cout << "\tPhase: " << phaseToString(m_currentPhase) << std::endl;
    std::cout << "\tCurrent Player: " << playerIdToString(m_currentPlayer) << std::endl;

    // run the simulation for the current state
    switch (m_currentPhase)
    {
        case Phase::Initiative:
            runInitiativePhase();
            break;
        case Phase::Hero:
            runHeroPhase();
            break;
        case Phase::Movement:
            runMovementPhase();
            break;
        case Phase::Shooting:
            runShootingPhase();
            break;
        case Phase::Charge:
            runChargePhase();
            break;
        case Phase::Combat:
            runCombatPhase();
            break;
        case Phase::Battleshock:
            runBattleshockPhase();
            break;
    }
}

void Battle::addPlayers(const Player &player1, const Player &player2)
{
    m_players[0] = player1;
    m_players[1] = player2;
}

void Battle::runInitiativePhase()
{
    Dice dice;
    // Roll D6 for each player, highest goes first.
    auto p1 = dice.rollD6();
    auto p2 = dice.rollD6();
    if (p1 == p2)
    {
        // Ties go to the player that went first in the previous round.
        m_currentPlayer = (m_currentPlayer == PlayerId::One) ? PlayerId::Two : PlayerId::One;
    }
    else if (p1 > p2)
    {
        m_currentPlayer = PlayerId::One;
    }
    else
    {
        m_currentPlayer = PlayerId::Two;
    }

    std::cout << "Player " << playerIdToString(m_currentPlayer) << " wins initiative.  P1: " <<
        p1 << " P2: " << p2 << std::endl;
}

void Battle::runHeroPhase()
{
    const auto playerIdx = (int)m_currentPlayer;
    m_players[playerIdx].doHeroPhase();
}

void Battle::runMovementPhase()
{
    const auto playerIdx = (int)m_currentPlayer;
    m_players[playerIdx].doMovementPhase();
}

void Battle::runShootingPhase()
{
    const auto playerIdx = (int)m_currentPlayer;
    m_players[playerIdx].doShootingPhase();
}

void Battle::runChargePhase()
{
    const auto playerIdx = (int)m_currentPlayer;
    m_players[playerIdx].doChargePhase();
}

void Battle::runCombatPhase()
{
    const auto playerIdx = (int)m_currentPlayer;
    m_players[playerIdx].doCombatPhase();
}

void Battle::runBattleshockPhase()
{
    const auto playerIdx = (int)m_currentPlayer;
    m_players[playerIdx].doBattleshockPhase();
}

std::string Battle::phaseToString(Phase phase) const
{
    switch (phase)
    {
        case Phase::Initiative:
            return "Initiative";
        case Phase::Hero:
            return "Hero";
        case Phase::Movement:
            return "Movement";
        case Phase::Shooting:
            return "Shooting";
        case Phase::Charge:
            return "Charge";
        case Phase::Combat:
            return "Combat";
        case Phase::Battleshock:
            return "Battleshock";
    }
}

std::string Battle::playerIdToString(PlayerId id) const
{
    switch (id)
    {
        case PlayerId::One:
            return "One";
        case PlayerId::Two:
            return "Two";
    }
}
