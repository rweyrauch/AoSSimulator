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

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

ManoAMano::ManoAMano(int numRounds, Realm realm) :
    m_numRounds(numRounds)
{
    auto board = Board::Instance();
    board->setSize(BoardWidth, BoardDepth);
    board->setRealm(realm);
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
    m_attackingPlayer = firstUnit;
    if (m_attackingPlayer == PlayerId::Red)
    {
        m_defendingPlayer = PlayerId::Blue;
    }
    else
    {
        m_defendingPlayer = PlayerId::Red;
    }
    m_currentPhase = Phase::Hero;
    m_round = 1;

    m_rosters[(int) m_attackingPlayer]->beginRound(m_round);
    m_rosters[(int) m_defendingPlayer]->beginRound(m_round);

    m_cpAvailable[(int) m_attackingPlayer]++;

    m_rosters[(int) m_attackingPlayer]->beginTurn(m_round, m_attackingPlayer);
    m_rosters[(int) m_defendingPlayer]->beginTurn(m_round, m_attackingPlayer);
}

void ManoAMano::simulate()
{
    SimLog(Verbosity::Narrative, "Fight State:\n");
    SimLog(Verbosity::Narrative, "\tRound: %d of %d.  Top of round: %d", m_round, m_numRounds, m_topOfRound);
    SimLog(Verbosity::Narrative, "\tPhase: %s\n", PhaseToString(m_currentPhase).c_str());
    SimLog(Verbosity::Narrative, "\tCurrent Unit: %s\n", PlayerIdToString(m_attackingPlayer).c_str());

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

                m_rosters[(int) m_attackingPlayer]->endTurn(m_round);
                m_rosters[(int) m_defendingPlayer]->endTurn(m_round);

                std::swap(m_attackingPlayer, m_defendingPlayer);

                m_cpAvailable[(int) m_attackingPlayer]++;

                m_rosters[(int) m_attackingPlayer]->beginTurn(m_round, m_attackingPlayer);
                m_rosters[(int) m_defendingPlayer]->beginTurn(m_round, m_attackingPlayer);

                m_currentPhase = Phase::Hero;
            }
            else
            {
                SimLog(Verbosity::Narrative, "At the end of round %d...\n", m_round);
                SimLog(Verbosity::Narrative, "\tTeam %s has %d remaining models with %d wounds remaining.\n",
                    PlayerIdToString(PlayerId::Red).c_str(), redUnit()->remainingModels(), redUnit()->remainingWounds());
                SimLog(Verbosity::Narrative, "\tTeam %s has %d remaining models with %d wounds remaining.\n",
                       PlayerIdToString(PlayerId::Blue).c_str(), blueUnit()->remainingModels(), blueUnit()->remainingWounds());

                m_rosters[(int) m_attackingPlayer]->endTurn(m_round);
                m_rosters[(int) m_defendingPlayer]->endTurn(m_round);

                m_rosters[(int) m_attackingPlayer]->endRound(m_round);
                m_rosters[(int) m_defendingPlayer]->endRound(m_round);

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
                    m_rosters[(int) m_attackingPlayer]->beginRound(m_round);
                    m_rosters[(int) m_defendingPlayer]->beginRound(m_round);

                    m_cpAvailable[(int) m_attackingPlayer]++;

                    m_rosters[(int) m_attackingPlayer]->beginTurn(m_round, m_attackingPlayer);
                    m_rosters[(int) m_defendingPlayer]->beginTurn(m_round, m_attackingPlayer);
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
        m_attackingPlayer = (m_attackingPlayer == PlayerId::Red) ? PlayerId::Blue : PlayerId::Red;
    }
    else if (p1 > p2)
    {
        m_attackingPlayer = PlayerId::Red;
    }
    else
    {
        m_attackingPlayer = PlayerId::Blue;
    }

    if (m_attackingPlayer == PlayerId::Red)
    {
        m_defendingPlayer = PlayerId::Blue;
    }
    else // attacker is blue
    {
        m_defendingPlayer = PlayerId::Red;
    }

    m_rosters[(int) m_attackingPlayer]->beginTurn(m_round, m_attackingPlayer);
    m_rosters[(int) m_defendingPlayer]->beginTurn(m_round, m_attackingPlayer);

    SimLog(Verbosity::Narrative, "Player %s wins initiative. Dice rolls: Red: %d  Blue: %d.\n",
        PlayerIdToString(m_attackingPlayer).c_str(), p1, p2);
}

void ManoAMano::runHeroPhase()
{
    SimLog(Verbosity::Narrative, "Starting player %s hero phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doHeroPhase();
}

void ManoAMano::runMovementPhase()
{
    SimLog(Verbosity::Narrative, "Starting player %s movement phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doMovementPhase();

    if (attackingUnit()->ran())
    {
        SimLog(Verbosity::Narrative, "%s:%s ran.\n", PlayerIdToString(m_attackingPlayer).c_str(), attackingUnit()->name().c_str());
    }
}

void ManoAMano::runShootingPhase()
{
    SimLog(Verbosity::Narrative, "Starting player %s shooting phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    // Think...
    m_rosters[(int) m_attackingPlayer]->doShootingPhase();

    // Act...
    int numSlain = 0;
    auto totalDamage = attackingUnit()->shoot(numSlain);

    SimLog(Verbosity::Narrative, "%s:%s did %d shooting damage to %s:%s slaying %d models.\n",
        PlayerIdToString(m_attackingPlayer).c_str(), attackingUnit()->name().c_str(), (totalDamage.normal + totalDamage.mortal),
        PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str(), numSlain);
}

void ManoAMano::runChargePhase()
{
    SimLog(Verbosity::Narrative, "Starting player %s charge phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doChargePhase();

    if (attackingUnit()->charged())
    {
        SimLog(Verbosity::Narrative, "%s:%s charged %s:%s.\n", PlayerIdToString(m_attackingPlayer).c_str(), attackingUnit()->name().c_str(),
            PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str());
    }
}

void ManoAMano::runCombatPhase()
{
    SimLog(Verbosity::Narrative, "Starting player %s combat phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    // Think.
    m_rosters[(int) m_attackingPlayer]->doCombatPhase();

    assert(attackingUnit()->meleeTarget() == defendingUnit());

    int numSlain = 0;
    auto totalDamage = attackingUnit()->fight(m_attackingPlayer, numSlain);

    SimLog(Verbosity::Narrative, "%s:%s did %d damage to %s:%s slaying %d models in the combat phase.\n", PlayerIdToString(m_attackingPlayer).c_str(),
           attackingUnit()->name().c_str(), (totalDamage.normal + totalDamage.mortal), PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str(),
           numSlain);

    numSlain = 0;
    totalDamage = defendingUnit()->fight(-1, attackingUnit(), numSlain);

    SimLog(Verbosity::Narrative, "%s:%s did %d damage to %s:%s slaying %d models in the counter attack.\n", PlayerIdToString(m_defendingPlayer).c_str(),
           defendingUnit()->name().c_str(), (totalDamage.normal + totalDamage.mortal), PlayerIdToString(m_attackingPlayer).c_str(), attackingUnit()->name().c_str(),
           numSlain);
}

void ManoAMano::runBattleshockPhase()
{
    SimLog(Verbosity::Narrative, "Starting player %s battleshock phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doBattleshockPhase();
    m_rosters[(int) m_defendingPlayer]->doBattleshockPhase();

    int numFleeing = attackingUnit()->applyBattleshock();
    if (numFleeing > 0)
    {
        SimLog(Verbosity::Narrative, "A total of %d %s from %s fled from battleshock.\n", numFleeing, attackingUnit()->name().c_str(),
            PlayerIdToString(m_attackingPlayer).c_str());
    }
    numFleeing = defendingUnit()->applyBattleshock();
    if (numFleeing > 0)
    {
        SimLog(Verbosity::Narrative, "A total of %d %s from %s fled from battleshock.\n", numFleeing, defendingUnit()->name().c_str(),
               PlayerIdToString(m_defendingPlayer).c_str());
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
    if (redUnit()->remainingPoints() / (float)redUnit()->points() > blueUnit()->remainingPoints() / (float)blueUnit()->points())
    {
        return PlayerId::Red;
    }
    // Blue suffered few losses
    if (redUnit()->remainingPoints() / (float)redUnit()->points() < blueUnit()->remainingPoints() / (float)blueUnit()->points())
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
    if (m_attackingPlayer == PlayerId::Red)
    {
        return redUnit();
    }
    else if (m_attackingPlayer == PlayerId::Blue)
    {
        return blueUnit();
    }
    return nullptr;
}

Unit *ManoAMano::defendingUnit()
{
    if (m_defendingPlayer == PlayerId::Red)
    {
        return redUnit();
    }
    else if (m_defendingPlayer == PlayerId::Blue)
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

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(manoamano_class) 
{
    emscripten::class_<ManoAMano>("ManoAMano")
        .constructor<int, Realm>()
        .function("combatants", &ManoAMano::combatants, emscripten::allow_raw_pointers())
        .function("start", &ManoAMano::start)
        .function("simulate", &ManoAMano::simulate)
        .function("next", &ManoAMano::next)
        .function("done", &ManoAMano::done)
        .function("getVictor", &ManoAMano::getVictor)
        ;
}
#endif // __EMSCRIPTEN__
