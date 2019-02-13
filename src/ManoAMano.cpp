/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <iostream>
#include <cassert>
#include <ManoAMano.h>
#include <Dice.h>
#include <Roster.h>
#include <Board.h>

ManoAMano::ManoAMano(int numRounds) :
    m_numRounds(numRounds)
{
    auto board = Board::Instance();
    board->setSize(BoardWidth, BoardDepth);
}

void ManoAMano::combatants(Unit *red, Unit *blue)
{
    auto board = Board::Instance();

    delete m_rosters[0];
    delete m_rosters[1];

    m_units[0] = red;
    m_units[1] = blue;

    m_rosters[0] = new Roster(PlayerId::Red);
    m_rosters[0]->addUnit(red);

    m_rosters[1] = new Roster(PlayerId::Blue);
    m_rosters[1]->addUnit(blue);

    // +-----------------------+
    // |                       |
    // |                       |
    // | red              blue |
    // |                       |
    // |                       |
    // +-----------------------+

    float redX = board->width() / 20.0f;
    float redY = board->depth() / 2.0f;

    // left center
    red->setPosition(Math::Point3(redX, redY, 0.0f), Math::Vector3(1.0f, 0.0f, 0.0f));

    float blueX = board->width() - (board->width() / 20.0f);
    float blueY = board->depth() / 2.0f;

    // right center
    blue->setPosition(Math::Point3(blueX, blueY, 0.0f), Math::Vector3(-1.0f, 0.0f, 0.0f));

    board->addRosters(m_rosters[0], m_rosters[1]);
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
    {
        firstUnit = PlayerId::Red;
    }
    else
    {
        firstUnit = PlayerId::Blue;
    }

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

    m_rosters[(int) m_attackingUnit]->beginTurn(m_round, m_attackingUnit);
    m_rosters[(int) m_defendingUnit]->beginTurn(m_round, m_attackingUnit);
}

void ManoAMano::simulate()
{
    /*
    if (m_verbose)
    {
        std::cout << "Fight State:" << std::endl;
        std::cout << "\tRound: " << m_round << " of " << m_numRounds << ".  Top of round: "
                  << m_topOfRound << std::endl;
        std::cout << "\tPhase: " << PhaseToString(m_currentPhase) << std::endl;
        std::cout << "\tCurrent Unit: " << PlayerIdToString(m_attackingUnit) << std::endl;
    }
    */

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

                m_rosters[(int) m_attackingUnit]->endTurn(m_round);
                m_rosters[(int) m_defendingUnit]->endTurn(m_round);

                std::swap(m_attackingUnit, m_defendingUnit);

                m_rosters[(int) m_attackingUnit]->beginTurn(m_round, m_attackingUnit);
                m_rosters[(int) m_defendingUnit]->beginTurn(m_round, m_attackingUnit);

                m_currentPhase = Phase::Hero;
            }
            else
            {
                if (GetVerbosity() == Verbosity::Narrative)
                {
                    std::cout << "A the end of round " << m_round << "..." << std::endl;
                    std::cout << "   Team " << PlayerIdToString(PlayerId::Red) << " has "
                              << m_units[0]->remainingModels() << " remaining models with "
                              << m_units[0]->remainingWounds() << " wounds remaining." << std::endl;
                    std::cout << "   Team " << PlayerIdToString(PlayerId::Blue) << " has "
                              << m_units[1]->remainingModels() << " remaining models with "
                              << m_units[1]->remainingWounds() << " wounds remaining." << std::endl;
                }

                m_rosters[(int) m_attackingUnit]->endTurn(m_round);
                m_rosters[(int) m_defendingUnit]->endTurn(m_round);

                // End of round.
                m_currentPhase = Phase::Initiative;
                m_topOfRound = true;
                m_round++;

                m_rosters[(int) m_attackingUnit]->beginTurn(m_round, m_attackingUnit);
                m_rosters[(int) m_defendingUnit]->beginTurn(m_round, m_attackingUnit);

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

    m_units[(int) m_attackingUnit]->beginTurn(m_round, m_attackingUnit);
    m_units[(int) m_defendingUnit]->beginTurn(m_round, m_defendingUnit);

    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Unit " << PlayerIdToString(m_attackingUnit) << " wins initiative.  Red: "
                  << p1 << " Blue: " << p2 << std::endl;
    }
}

void ManoAMano::runHeroPhase()
{
    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Starting player " << PlayerIdToString(m_attackingUnit) << " hero phase." << std::endl;
    }

    m_units[(int) m_attackingUnit]->hero(m_attackingUnit);
}

void ManoAMano::runMovementPhase()
{
    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Starting player " << PlayerIdToString(m_attackingUnit) << " movement phase." << std::endl;
    }

    m_units[(int) m_attackingUnit]->movement(m_attackingUnit);

    if (GetVerbosity() == Verbosity::Narrative)
    {
        if (m_units[(int) m_attackingUnit]->ran())
        {
            std::cout << PlayerIdToString(m_attackingUnit) << ":" << m_units[(int) m_attackingUnit]->name()
                      << " ran." << std::endl;
        }
    }
}

void ManoAMano::runShootingPhase()
{
    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Starting player " << PlayerIdToString(m_attackingUnit) << " shooting phase." << std::endl;
    }

    // Think...
    m_units[(int) m_attackingUnit]->shooting(m_attackingUnit);

    // Act...
    int numSlain = 0;
    auto totalDamage = m_units[(int) m_attackingUnit]->shoot(numSlain);
    if (totalDamage.normal > 0 || totalDamage.mortal > 0)
    {
        if (GetVerbosity() == Verbosity::Narrative)
        {
            std::cout << PlayerIdToString(m_attackingUnit) << ":"
                      << m_units[(int) m_attackingUnit]->name()
                      << " did " << (totalDamage.normal + totalDamage.mortal) << " shooting damage to "
                      << PlayerIdToString(m_defendingUnit)
                      << ":" << m_units[(int) m_defendingUnit]->name() << " slaying " << numSlain
                      << " models. " << std::endl;
        }
    }
}

void ManoAMano::runChargePhase()
{
    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Starting player " << PlayerIdToString(m_attackingUnit) << " charge phase." << std::endl;
    }

    m_units[(int) m_attackingUnit]->charge(m_attackingUnit);

    if (GetVerbosity() == Verbosity::Narrative)
    {
        if (m_units[(int) m_attackingUnit]->charged())
        {
            std::cout << PlayerIdToString(m_attackingUnit) << ":" << m_units[(int) m_attackingUnit]->name()
                      << " charged " << PlayerIdToString(m_defendingUnit) << ":" << m_units[(int) m_defendingUnit]->name() << std::endl;
        }
    }
}

void ManoAMano::runCombatPhase()
{
    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Starting player " << PlayerIdToString(m_attackingUnit) << " combat phase." << std::endl;
    }

    // Think.
    m_units[(int) m_attackingUnit]->combat(m_attackingUnit);

    assert(m_units[(int) m_attackingUnit]->meleeTarget() == m_units[(int) m_defendingUnit]);

    int numSlain = 0;
    auto totalDamage = m_units[(int) m_attackingUnit]->fight(m_attackingUnit, numSlain);

    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << PlayerIdToString(m_attackingUnit) << ":"
                  << m_units[(int) m_attackingUnit]->name()
                  << " did " << (totalDamage.normal + totalDamage.mortal) << " damage to " << PlayerIdToString(m_defendingUnit)
                  << ":" << m_units[(int) m_defendingUnit]->name() << " slaying " << numSlain
                  << " models in the combat phase." << std::endl;
    }
    numSlain = 0;
    totalDamage = m_units[(int) m_defendingUnit]->fight(-1, m_units[(int) m_attackingUnit], numSlain);

    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << PlayerIdToString(m_defendingUnit) << ":"
                  << m_units[(int) m_defendingUnit]->name()
                  << " did " << (totalDamage.normal + totalDamage.mortal) << " damage to " << PlayerIdToString(m_attackingUnit)
                  << ":" << m_units[(int) m_attackingUnit]->name() << " slaying " << numSlain
                  << " model in the counter attack." << std::endl;
    }
}

void ManoAMano::runBattleshockPhase()
{
    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Starting player " << PlayerIdToString(m_attackingUnit) << " battleshock phase." << std::endl;
    }

    m_units[(int) m_attackingUnit]->battleshock(m_attackingUnit);
    m_units[(int) m_defendingUnit]->battleshock(m_defendingUnit);

    int numFleeing = m_units[(int) m_attackingUnit]->applyBattleshock();
    if (numFleeing > 0)
    {
        if (GetVerbosity() == Verbosity::Narrative)
        {
            std::cout << "A total of " << numFleeing << " "
                      << m_units[(int) m_attackingUnit]->name() << " from "
                      << PlayerIdToString(m_attackingUnit) << " fled from battleshock."
                      << std::endl;
        }
    }
    numFleeing = m_units[(int) m_defendingUnit]->applyBattleshock();
    if (numFleeing > 0)
    {
        if (GetVerbosity() == Verbosity::Narrative)
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
    {
        return PlayerId::Blue;
    }
    if (m_units[0]->remainingModels() > 0 && m_units[1]->remainingModels() == 0)
    {
        return PlayerId::Red;
    }
    if (m_units[0]->remainingWounds() > m_units[1]->remainingWounds())
    {
        return PlayerId::Red;
    }
    if (m_units[0]->remainingWounds() < m_units[1]->remainingWounds())
    {
        return PlayerId::Blue;
    }

    return PlayerId::None;
}

static void logUnitStats(const UnitStatistics &stats)
{
    std::cout << "\tTotal Movement: " << stats.totalMovementDistance() << "  Rounds Moved: " << stats.numberOfRoundsMoved() << std::endl;
    std::cout << "\tTotal Run Distance: " << stats.totalRunDistance() << "  Rounds Ran: " << stats.numberOfRoundsRan() << std::endl;
    std::cout << "\tTotal Charge Distance: " << stats.totalChargeDistance() << "  Rounds Charged: " << stats.numberOfRoundsCharged() << std::endl;
    std::cout << "\tTotal Enemy Models Slain: " << stats.totalEnemyModelsSlain() << "  Wounds Inflicted: " << stats.totalWoundsInflicted().normal
              << ", " << stats.totalWoundsInflicted().mortal << std::endl;
    std::cout << "\tTotal Models Slain: " << stats.totalModelsSlain() << "  Wounds Taken: " << stats.totalWoundsTaken().normal
              << ", " << stats.totalWoundsTaken().mortal << std::endl;
    std::cout << "\tTotal Models Fled: " << stats.totalModelsFled() << std::endl;
}

void ManoAMano::logStatistics() const
{
    std::function<void(const TurnRecord &)> turnVistor = [](const TurnRecord &turn)
    {
        std::cout << "\tTurn " << turn.m_round << "  Player: " << PlayerIdToString(turn.m_playerWithTurn) << std::endl;
        std::cout << "\t\tMoved: " << turn.m_moved << "  Ran: " << turn.m_ran << " Charged: " << turn.m_charged << std::endl;
        std::cout << "\t\tAttacks Made: " << turn.m_attacksMade << "  Attacks Hit: " << turn.m_attacksHitting << std::endl;
        std::cout << "\t\tEnemy Slain: " << turn.m_enemyModelsSlain << "  Wounds Inflicted: " << turn.m_woundsInflicted.normal
                  << ", " << turn.m_woundsInflicted.mortal << std::endl;
        std::cout << "\t\tSaves Made: " << turn.m_savesMade << " Failed: " << turn.m_savesFailed << std::endl;
        std::cout << "\t\tModel Slain: " << turn.m_modelsSlain << "  Wounds Taken: " << turn.m_woundsTaken.normal
                  << ", " << turn.m_woundsTaken.mortal << std::endl;
    };

    auto redStats = m_units[0]->getStatistics();
    std::cout << "Red Statistics:" << std::endl;
    logUnitStats(redStats);
    redStats.visitTurn(turnVistor);

    auto blueStats = m_units[1]->getStatistics();
    std::cout << "Blue Statistics:" << std::endl;
    logUnitStats(blueStats);
    blueStats.visitTurn(turnVistor);
}
