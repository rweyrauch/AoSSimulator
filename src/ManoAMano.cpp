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
#include <gloomspitegitz/BadMoon.h>

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
    m_initialPos[0] = Math::Point3(redX, redY, 0.0f);
    red->setPosition(m_initialPos[0], Math::Vector3(1.0f, 0.0f, 0.0f));

    float blueX = board->width() - (board->width() / 20.0f);
    float blueY = board->depth() / 2.0f;

    // right center
    m_initialPos[1] = Math::Point3(blueX, blueY, 0.0f);
    blue->setPosition(m_initialPos[1], Math::Vector3(-1.0f, 0.0f, 0.0f));

    board->addRosters(m_rosters[0], m_rosters[1]);

    // Setup a Bad Moon if a player has a Gloomspite Gitz unit.
    if (red->hasKeyword(GLOOMSPITE_GITZ))
    {
        GloomspiteGitz::BadMoon::Instance()->setup(GloomspiteGitz::BadMoon::Location::Northwest);
    }
    else if (blue->hasKeyword(GLOOMSPITE_GITZ))
    {
        GloomspiteGitz::BadMoon::Instance()->setup(GloomspiteGitz::BadMoon::Location::Northeast);
    }
}

void ManoAMano::start()
{
    Dice dice;

    redUnit()->setPosition(m_initialPos[0], Math::Vector3(1.0f, 0.0f, 0.0f));
    blueUnit()->setPosition(m_initialPos[1], Math::Vector3(-1.0f, 0.0f, 0.0f));

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

    m_rosters[(int) m_attackingUnit]->beginRound(m_round);
    m_rosters[(int) m_defendingUnit]->beginRound(m_round);

    m_cpAvailable[(int) m_attackingUnit]++;

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

                m_cpAvailable[(int) m_attackingUnit]++;

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
                              << redUnit()->remainingModels() << " remaining models with "
                              << redUnit()->remainingWounds() << " wounds remaining." << std::endl;
                    std::cout << "   Team " << PlayerIdToString(PlayerId::Blue) << " has "
                              << blueUnit()->remainingModels() << " remaining models with "
                              << blueUnit()->remainingWounds() << " wounds remaining." << std::endl;
                }

                m_rosters[(int) m_attackingUnit]->endTurn(m_round);
                m_rosters[(int) m_defendingUnit]->endTurn(m_round);

                m_rosters[(int) m_attackingUnit]->endRound(m_round);
                m_rosters[(int) m_defendingUnit]->endRound(m_round);

                // End of round.
                m_currentPhase = Phase::Initiative;
                m_topOfRound = true;
                m_round++;

                // End of battle.
                if (m_round > m_numRounds)
                {
                    m_isDone = true;
                }
                else
                {
                    m_rosters[(int) m_attackingUnit]->beginRound(m_round);
                    m_rosters[(int) m_defendingUnit]->beginRound(m_round);

                    m_cpAvailable[(int) m_attackingUnit]++;

                    m_rosters[(int) m_attackingUnit]->beginTurn(m_round, m_attackingUnit);
                    m_rosters[(int) m_defendingUnit]->beginTurn(m_round, m_attackingUnit);
                }
            }
            // Check for a victory
            if (redUnit()->remainingModels() == 0 || blueUnit()->remainingModels() == 0)
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

    m_rosters[(int) m_attackingUnit]->beginTurn(m_round, m_attackingUnit);
    m_rosters[(int) m_defendingUnit]->beginTurn(m_round, m_attackingUnit);

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

    m_rosters[(int) m_attackingUnit]->doHeroPhase(m_cpAvailable[(int) m_attackingUnit]);
}

void ManoAMano::runMovementPhase()
{
    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Starting player " << PlayerIdToString(m_attackingUnit) << " movement phase." << std::endl;
    }

    m_rosters[(int) m_attackingUnit]->doMovementPhase();

    if (GetVerbosity() == Verbosity::Narrative)
    {

        if (attackingUnit()->ran())
        {
            std::cout << PlayerIdToString(m_attackingUnit) << ":" << attackingUnit()->name()
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
    m_rosters[(int) m_attackingUnit]->doShootingPhase();

    // Act...
    int numSlain = 0;
    auto totalDamage = attackingUnit()->shoot(numSlain);
    if (totalDamage.normal > 0 || totalDamage.mortal > 0)
    {
        if (GetVerbosity() == Verbosity::Narrative)
        {
            std::cout << PlayerIdToString(m_attackingUnit) << ":"
                      << attackingUnit()->name()
                      << " did " << (totalDamage.normal + totalDamage.mortal) << " shooting damage to "
                      << PlayerIdToString(m_defendingUnit)
                      << ":" << defendingUnit()->name() << " slaying " << numSlain
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

    m_rosters[(int) m_attackingUnit]->doChargePhase();

    if (GetVerbosity() == Verbosity::Narrative)
    {
        if (attackingUnit()->charged())
        {
            std::cout << PlayerIdToString(m_attackingUnit) << ":" << attackingUnit()->name()
                      << " charged " << PlayerIdToString(m_defendingUnit) << ":" << defendingUnit()->name() << std::endl;
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
    m_rosters[(int) m_attackingUnit]->doCombatPhase();

    assert(attackingUnit()->meleeTarget() == defendingUnit());

    int numSlain = 0;
    auto totalDamage = attackingUnit()->fight(m_attackingUnit, numSlain);

    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << PlayerIdToString(m_attackingUnit) << ":"
                  << attackingUnit()->name()
                  << " did " << (totalDamage.normal + totalDamage.mortal) << " damage to " << PlayerIdToString(m_defendingUnit)
                  << ":" << defendingUnit()->name() << " slaying " << numSlain
                  << " models in the combat phase." << std::endl;
    }
    numSlain = 0;
    totalDamage = defendingUnit()->fight(-1, attackingUnit(), numSlain);

    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << PlayerIdToString(m_defendingUnit) << ":"
                  << defendingUnit()->name()
                  << " did " << (totalDamage.normal + totalDamage.mortal) << " damage to " << PlayerIdToString(m_attackingUnit)
                  << ":" << attackingUnit()->name() << " slaying " << numSlain
                  << " model in the counter attack." << std::endl;
    }
}

void ManoAMano::runBattleshockPhase()
{
    if (GetVerbosity() == Verbosity::Narrative)
    {
        std::cout << "Starting player " << PlayerIdToString(m_attackingUnit) << " battleshock phase." << std::endl;
    }

    m_rosters[(int) m_attackingUnit]->doBattleshockPhase();
    m_rosters[(int) m_defendingUnit]->doBattleshockPhase();

    int numFleeing = attackingUnit()->applyBattleshock();
    if (numFleeing > 0)
    {
        if (GetVerbosity() == Verbosity::Narrative)
        {
            std::cout << "A total of " << numFleeing << " "
                      << attackingUnit()->name() << " from "
                      << PlayerIdToString(m_attackingUnit) << " fled from battleshock."
                      << std::endl;
        }
    }
    numFleeing = defendingUnit()->applyBattleshock();
    if (numFleeing > 0)
    {
        if (GetVerbosity() == Verbosity::Narrative)
        {
            std::cout << "A total of " << numFleeing << " "
                      << defendingUnit()->name() << " from "
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
    // Blue 'tabled' Red
    if (redUnit()->remainingModels() == 0 && blueUnit()->remainingModels() > 0)
    {
        return PlayerId::Blue;
    }
    // Red 'tabled' Blue
    if (redUnit()->remainingModels() > 0 && blueUnit()->remainingModels() == 0)
    {
        return PlayerId::Red;
    }
    // Red suffered fewer losses
    if (redUnit()->remainingPoints() > blueUnit()->remainingPoints())
    {
        return PlayerId::Red;
    }
    // Blue suffered few losses
    if (redUnit()->remainingPoints() < blueUnit()->remainingPoints())
    {
        return PlayerId::Blue;
    }

    // Tie
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

    auto redStats = redUnit()->getStatistics();
    std::cout << "Red Statistics:" << std::endl;
    logUnitStats(redStats);
    if (GetVerbosity() == Verbosity::Narrative)
        redStats.visitTurn(turnVistor);

    auto blueStats = blueUnit()->getStatistics();
    std::cout << "Blue Statistics:" << std::endl;
    logUnitStats(blueStats);
    if (GetVerbosity() == Verbosity::Narrative)
        blueStats.visitTurn(turnVistor);
}

Unit *ManoAMano::redUnit()
{
    auto red = m_rosters[0]->unitBegin();
    return *red;
}

Unit *ManoAMano::blueUnit()
{
    auto blue = m_rosters[1]->unitBegin();
    return *blue;
}

Unit *ManoAMano::attackingUnit()
{
    if (m_attackingUnit == PlayerId::Red)
    {
        return redUnit();
    }
    else if (m_attackingUnit == PlayerId::Blue)
    {
        return blueUnit();
    }
    return nullptr;
}

Unit *ManoAMano::defendingUnit()
{
    if (m_defendingUnit == PlayerId::Red)
    {
        return redUnit();
    }
    else if (m_defendingUnit == PlayerId::Blue)
    {
        return blueUnit();
    }
    return nullptr;
}

const Unit *ManoAMano::redUnit() const
{
    const auto red = m_rosters[0]->unitBegin();
    return *red;
}

const Unit *ManoAMano::blueUnit() const
{
    const auto blue = m_rosters[1]->unitBegin();
    return *blue;
}
