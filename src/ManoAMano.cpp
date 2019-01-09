/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <iostream>
#include <ManoAMano.h>
#include <Dice.h>
#include <Roster.h>

ManoAMano::ManoAMano(int numRounds, bool verbose) :
    m_numRounds(numRounds),
    m_verbose(verbose)
{
}

void ManoAMano::combatants(Unit *red, Unit *blue)
{
    delete m_rosters[0];
    delete m_rosters[1];

    m_units[0] = red;
    m_units[1] = blue;

    m_rosters[0] = new Roster();
    m_rosters[0]->addUnit(red);

    m_rosters[1] = new Roster();
    m_rosters[1]->addUnit(blue);
}

void ManoAMano::start()
{
    Dice dice;

    int redRoll = 0;
    int blueRoll = 0;
    while (redRoll == blueRoll)
    {
        redRoll = dice.rollD6();
        blueRoll = dice.rollD6();
    }
    PlayerId firstUnit = PlayerId::None;
    if (redRoll > blueRoll)
        firstUnit = PlayerId::Red;
    else
        firstUnit = PlayerId::Blue;

    m_isDone = false;
    m_topOfRound = true;
    m_attackingUnit = firstUnit;
    if (m_attackingUnit == PlayerId::Red)
    {
        m_defendingUnit = PlayerId::Blue;
    }
    else
    {
        m_defendingUnit = PlayerId::Red;
    }
    m_currentPhase = Phase::Hero;
    m_round = 1;


}

void ManoAMano::simulate()
{
    //std::cout << "Fight State:" << std::endl;
    //std::cout << "\tRound: " << m_round << " of " << m_numRounds << ".  Top of round: " << m_topOfRound << std::endl;
    //std::cout << "\tPhase: " << PhaseToString(m_currentPhase) << std::endl;
    //std::cout << "\tCurrent Unit: " << PlayerIdToString(m_attackingUnit) << std::endl;

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

void ManoAMano::next()
{
    // advance fight state machine
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
                // Next unit's turn
                m_topOfRound = false;
                std::swap(m_attackingUnit, m_defendingUnit);

                m_units[(int)m_attackingUnit]->beginTurn();
                m_units[(int)m_defendingUnit]->beginTurn();

                m_currentPhase = Phase::Hero;
            }
            else
            {
                if (m_verbose)
                {
                    std::cout << "A the end of round " << m_round << "..." << std::endl;
                    std::cout << "   Team " << PlayerIdToString(PlayerId::Red) << " has "
                              << m_units[0]->remainingModels() << " remaining model." << std::endl;
                    std::cout << "   Team " << PlayerIdToString(PlayerId::Blue) << " has "
                              << m_units[1]->remainingModels() << " remaining models." << std::endl;
                }

                // End of round.
                m_currentPhase = Phase::Initiative;
                m_topOfRound = true;
                m_round++;

                m_units[(int)m_attackingUnit]->beginTurn();
                m_units[(int)m_defendingUnit]->beginTurn();

                // End of battle.
                if (m_round > m_numRounds)
                {
                    m_isDone = true;
                }
            }
            // Check for a victory
            if (m_units[0]->remainingModels() == 0 || m_units[1]->remainingModels() == 0)
            {
                m_isDone = true;
            }
            break;
    }
}

bool ManoAMano::done()
{
    return m_isDone;
}

void ManoAMano::runInitiativePhase()
{
    Dice dice;
    // Roll D6 for each player, highest goes first.
    auto p1 = dice.rollD6();
    auto p2 = dice.rollD6();
    if (p1 == p2)
    {
        // Ties go to the player that went first in the previous round.
        m_attackingUnit = (m_attackingUnit == PlayerId::Red) ? PlayerId::Blue : PlayerId::Red;
    }
    else if (p1 > p2)
    {
        m_attackingUnit = PlayerId::Red;
    }
    else
    {
        m_attackingUnit = PlayerId::Blue;
    }

    if (m_attackingUnit == PlayerId::Red)
    {
        m_defendingUnit = PlayerId::Blue;
    }
    else // attacker is blue
    {
        m_defendingUnit = PlayerId::Red;
    }

    const auto unitIdx = (int)m_attackingUnit;
    m_units[unitIdx]->beginTurn();

    //std::cout << "Unit " << PlayerIdToString(m_attackingUnit) << " wins initiative.  Red: " <<
     //         p1 << " Blue: " << p2 << std::endl;
}

void ManoAMano::runHeroPhase()
{
    m_units[(int)m_attackingUnit]->hero();
}

void ManoAMano::runMovementPhase()
{
    m_units[(int)m_attackingUnit]->movement();
}

void ManoAMano::runShootingPhase()
{
    m_units[(int)m_attackingUnit]->shooting();

    int numSlain = 0;
    int totalDamage = m_units[(int)m_attackingUnit]->shoot(-1, m_units[(int)m_defendingUnit], numSlain);
    if (totalDamage > 0)
    {
        if (m_verbose)
        {
            std::cout << PlayerIdToString(m_attackingUnit) << ":"
                      << m_units[(int) m_attackingUnit]->name()
                      << " did " << totalDamage << " shooting damage to "
                      << PlayerIdToString(m_defendingUnit)
                      << ":" << m_units[(int) m_defendingUnit]->name() << " slaying " << numSlain
                      << " models. " << std::endl;
        }
    }
}

void ManoAMano::runChargePhase()
{
    m_units[(int)m_attackingUnit]->charge();
}

void ManoAMano::runCombatPhase()
{
    m_units[(int)m_attackingUnit]->combat();

    int numSlain = 0;
    int totalDamage = m_units[(int)m_attackingUnit]->fight(-1, m_units[(int)m_defendingUnit], numSlain);

    if (m_verbose)
    {
        std::cout << PlayerIdToString(m_attackingUnit) << ":"
                  << m_units[(int) m_attackingUnit]->name()
                  << " did " << totalDamage << " damage to " << PlayerIdToString(m_defendingUnit)
                  << ":" << m_units[(int) m_defendingUnit]->name() << " slaying " << numSlain
                  << " models in the combat phase." << std::endl;
    }
    numSlain = 0;
    totalDamage = m_units[(int)m_defendingUnit]->fight(-1, m_units[(int)m_attackingUnit], numSlain);

    if (m_verbose)
    {
        std::cout << PlayerIdToString(m_defendingUnit) << ":"
                  << m_units[(int) m_defendingUnit]->name()
                  << " did " << totalDamage << " damage to " << PlayerIdToString(m_attackingUnit)
                  << ":" << m_units[(int) m_attackingUnit]->name() << " slaying " << numSlain
                  << " model in the counter attack." << std::endl;
    }
}

void ManoAMano::runBattleshockPhase()
{
    m_units[(int)m_attackingUnit]->battleshock();
    m_units[(int)m_defendingUnit]->battleshock();

    int numFleeing = m_units[(int)m_attackingUnit]->applyBattleshock();
    if (numFleeing > 0)
    {
        if (m_verbose)
        {
            std::cout << "A total of " << numFleeing << " "
                      << m_units[(int) m_attackingUnit]->name() << " from "
                      << PlayerIdToString(m_attackingUnit) << " fled from battleshock."
                      << std::endl;
        }
    }
    numFleeing = m_units[(int)m_defendingUnit]->applyBattleshock();
    if (numFleeing > 0)
    {
        if (m_verbose)
        {
            std::cout << "A total of " << numFleeing << " "
                      << m_units[(int) m_defendingUnit]->name() << " from "
                      << PlayerIdToString(m_defendingUnit) << " fled from battleshock."
                      << std::endl;
        }
    }

}

ManoAMano::~ManoAMano()
{
    delete m_rosters[0];
    delete m_rosters[1];
}

PlayerId ManoAMano::getVictor() const
{
    if (m_units[0]->remainingModels() == 0 && m_units[1]->remainingModels() > 0)
        return PlayerId::Blue;
    if (m_units[0]->remainingModels() > 0 && m_units[1]->remainingModels() == 0)
        return  PlayerId::Red;
    if (m_units[0]->remainingWounds() > m_units[1]->remainingWounds())
        return PlayerId::Red;
    if (m_units[0]->remainingWounds() < m_units[1]->remainingWounds())
        return PlayerId::Blue;

    return PlayerId::None;
}
