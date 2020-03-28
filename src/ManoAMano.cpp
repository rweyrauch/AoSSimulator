/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <ManoAMano.h>
#include <Dice.h>
#include <Roster.h>
#include <Board.h>
#include <gloomspitegitz/BadMoon.h>

ManoAMano::ManoAMano(int numRounds, Realm realm) :
        m_numRounds(numRounds) {
    auto board = Board::Instance();
    board->setSize(BoardWidth, BoardDepth);
    board->setRealm(realm);
}

void ManoAMano::combatants(Unit *red, Unit *blue) {
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

    float redX = board->width() / 10.0f;
    float redY = board->depth() / 2.0f;

    // left center
    m_initialPos[0] = Math::Point3(redX, redY, 0.0f);
    red->setPosition(m_initialPos[0], Math::Vector3(1.0f, 0.0f, 0.0f));

    float blueX = board->width() - (board->width() / 10.0f);
    float blueY = board->depth() / 2.0f;

    // right center
    m_initialPos[1] = Math::Point3(blueX, blueY, 0.0f);
    blue->setPosition(m_initialPos[1], Math::Vector3(-1.0f, 0.0f, 0.0f));

    board->addRosters(m_rosters[0], m_rosters[1]);

    // Setup a Bad Moon if a player has a Gloomspite Gitz unit.
    if (red->hasKeyword(GLOOMSPITE_GITZ)) {
        GloomspiteGitz::BadMoon::Instance()->setup(GloomspiteGitz::BadMoon::Location::Northwest);
    } else if (blue->hasKeyword(GLOOMSPITE_GITZ)) {
        GloomspiteGitz::BadMoon::Instance()->setup(GloomspiteGitz::BadMoon::Location::Northeast);
    }
}

void ManoAMano::start() {
    redUnit()->setPosition(m_initialPos[0], Math::Vector3(1.0f, 0.0f, 0.0f));
    blueUnit()->setPosition(m_initialPos[1], Math::Vector3(-1.0f, 0.0f, 0.0f));

    int redRoll = 0;
    int blueRoll = 0;
    while (redRoll == blueRoll) {
        redRoll = Dice::rollD6();
        blueRoll = Dice::rollD6();
    }
    PlayerId firstUnit = PlayerId::Blue;
    if (redRoll > blueRoll) {
        firstUnit = PlayerId::Red;
    }

    m_isDone = false;
    m_topOfRound = true;
    m_attackingPlayer = firstUnit;
    if (m_attackingPlayer == PlayerId::Red) {
        m_defendingPlayer = PlayerId::Blue;
    } else {
        m_defendingPlayer = PlayerId::Red;
    }
    m_currentPhase = Phase::Hero;
    m_round = 1;

    m_rosters[(int) m_attackingPlayer]->beginRound(m_round);
    m_rosters[(int) m_defendingPlayer]->beginRound(m_round);

    m_cpAvailable[(int) m_attackingPlayer]++;
    m_rosters[(int) m_attackingPlayer]->setCommandPoints(m_cpAvailable[(int) m_attackingPlayer]);

    m_rosters[(int) m_attackingPlayer]->beginTurn(m_round, m_attackingPlayer);
    m_rosters[(int) m_defendingPlayer]->beginTurn(m_round, m_attackingPlayer);
}

void ManoAMano::simulate() {
    SimLog(Verbosity::Narrative, "Fight State:\n");
    SimLog(Verbosity::Narrative, "\tRound: %d of %d.  Top of round: %d", m_round, m_numRounds, m_topOfRound);
    SimLog(Verbosity::Narrative, "\tPhase: %s\n", PhaseToString(m_currentPhase).c_str());
    SimLog(Verbosity::Narrative, "\tCurrent Unit: %s\n", PlayerIdToString(m_attackingPlayer).c_str());

    // run the simulation for the current state
    switch (m_currentPhase) {
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

void ManoAMano::next() {

    // advance fight state machine
    switch (m_currentPhase) {
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
            if (m_topOfRound) {
                // Next unit's turn
                m_topOfRound = false;

                m_rosters[(int) m_attackingPlayer]->endTurn(m_round);
                m_rosters[(int) m_defendingPlayer]->endTurn(m_round);

                std::swap(m_attackingPlayer, m_defendingPlayer);

                m_cpAvailable[(int) m_attackingPlayer]++;
                m_rosters[(int) m_attackingPlayer]->setCommandPoints(m_cpAvailable[(int) m_attackingPlayer]);

                m_rosters[(int) m_attackingPlayer]->beginTurn(m_round, m_attackingPlayer);
                m_rosters[(int) m_defendingPlayer]->beginTurn(m_round, m_attackingPlayer);

                m_currentPhase = Phase::Hero;
            } else {
                SimLog(Verbosity::Narrative, "At the end of round %d...\n", m_round);
                SimLog(Verbosity::Narrative, "\tTeam %s has %d remaining models with %d wounds remaining.\n",
                       PlayerIdToString(PlayerId::Red).c_str(), redUnit()->remainingModels(),
                       redUnit()->remainingWounds());
                SimLog(Verbosity::Narrative, "\tTeam %s has %d remaining models with %d wounds remaining.\n",
                       PlayerIdToString(PlayerId::Blue).c_str(), blueUnit()->remainingModels(),
                       blueUnit()->remainingWounds());

                m_rosters[(int) m_attackingPlayer]->endTurn(m_round);
                m_rosters[(int) m_defendingPlayer]->endTurn(m_round);

                m_rosters[(int) m_attackingPlayer]->endRound(m_round);
                m_rosters[(int) m_defendingPlayer]->endRound(m_round);

                // End of round.
                m_currentPhase = Phase::Initiative;
                m_topOfRound = true;
                m_round++;

                // End of battle.
                if (m_round > m_numRounds) {
                    m_isDone = true;
                } else {
                    m_rosters[(int) m_attackingPlayer]->beginRound(m_round);
                    m_rosters[(int) m_defendingPlayer]->beginRound(m_round);

                    m_cpAvailable[(int) m_attackingPlayer]++;
                    m_rosters[(int) m_attackingPlayer]->setCommandPoints(m_cpAvailable[(int) m_attackingPlayer]);

                    m_rosters[(int) m_attackingPlayer]->beginTurn(m_round, m_attackingPlayer);
                    m_rosters[(int) m_defendingPlayer]->beginTurn(m_round, m_attackingPlayer);
                }
            }
            // Check for a victory
            if (redUnit()->remainingModels() == 0 || blueUnit()->remainingModels() == 0) {
                m_isDone = true;
            }
            break;
    }
}

bool ManoAMano::done() {
    return m_isDone;
}

void ManoAMano::runInitiativePhase() {
    // Roll D6 for each player, highest goes first.
    auto p1 = Dice::rollD6();
    auto p2 = Dice::rollD6();
    if (p1 == p2) {
        // Ties go to the player that went first in the previous round.
        m_attackingPlayer = (m_attackingPlayer == PlayerId::Red) ? PlayerId::Blue : PlayerId::Red;
    } else if (p1 > p2) {
        m_attackingPlayer = PlayerId::Red;
    } else {
        m_attackingPlayer = PlayerId::Blue;
    }

    if (m_attackingPlayer == PlayerId::Red) {
        m_defendingPlayer = PlayerId::Blue;
    } else // attacker is blue
    {
        m_defendingPlayer = PlayerId::Red;
    }

    m_rosters[(int) m_attackingPlayer]->beginTurn(m_round, m_attackingPlayer);
    m_rosters[(int) m_defendingPlayer]->beginTurn(m_round, m_attackingPlayer);

    SimLog(Verbosity::Narrative, "Player %s wins initiative. Dice rolls: Red: %d  Blue: %d.\n",
           PlayerIdToString(m_attackingPlayer).c_str(), p1, p2);
}

void ManoAMano::runHeroPhase() {
    SimLog(Verbosity::Narrative, "Starting player %s hero phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doHeroPhase();
}

void ManoAMano::runMovementPhase() {
    SimLog(Verbosity::Narrative, "Starting player %s movement phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doMovementPhase();

    if (attackingUnit()->ran()) {
        SimLog(Verbosity::Narrative, "%s:%s ran.\n", PlayerIdToString(m_attackingPlayer).c_str(),
               attackingUnit()->name().c_str());
    }
}

void ManoAMano::runShootingPhase() {
    SimLog(Verbosity::Narrative, "Starting player %s shooting phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    // Think...
    m_rosters[(int) m_attackingPlayer]->doShootingPhase();

    // Act...
    int numSlain = 0;
    auto totalDamage = attackingUnit()->shoot(numSlain);

    SimLog(Verbosity::Narrative, "%s:%s did %d shooting damage to %s:%s slaying %d models.\n",
           PlayerIdToString(m_attackingPlayer).c_str(), attackingUnit()->name().c_str(),
           (totalDamage.normal + totalDamage.mortal),
           PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str(), numSlain);
}

void ManoAMano::runChargePhase() {
    SimLog(Verbosity::Narrative, "Starting player %s charge phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doChargePhase();

    if (attackingUnit()->charged()) {
        SimLog(Verbosity::Narrative, "%s:%s charged %s:%s.\n", PlayerIdToString(m_attackingPlayer).c_str(),
               attackingUnit()->name().c_str(),
               PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str());
    }
}

void ManoAMano::runCombatPhase() {
    SimLog(Verbosity::Narrative, "Starting player %s combat phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    // Think.
    m_rosters[(int) m_attackingPlayer]->doCombatPhase();

    //assert(attackingUnit()->meleeTarget() == defendingUnit());

    int numSlain = 0;
    auto totalDamage = attackingUnit()->fight(m_attackingPlayer, numSlain);

    SimLog(Verbosity::Narrative, "%s:%s did %d damage to %s:%s slaying %d models in the combat phase.\n",
           PlayerIdToString(m_attackingPlayer).c_str(),
           attackingUnit()->name().c_str(), (totalDamage.normal + totalDamage.mortal),
           PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str(),
           numSlain);

    numSlain = 0;
    totalDamage = defendingUnit()->fight(-1, attackingUnit(), numSlain);

    SimLog(Verbosity::Narrative, "%s:%s did %d damage to %s:%s slaying %d models in the counter attack.\n",
           PlayerIdToString(m_defendingPlayer).c_str(),
           defendingUnit()->name().c_str(), (totalDamage.normal + totalDamage.mortal),
           PlayerIdToString(m_attackingPlayer).c_str(), attackingUnit()->name().c_str(),
           numSlain);
}

void ManoAMano::runBattleshockPhase() {
    SimLog(Verbosity::Narrative, "Starting player %s battleshock phase.\n",
           PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doBattleshockPhase();
    m_rosters[(int) m_defendingPlayer]->doBattleshockPhase();

    int numFleeing = attackingUnit()->applyBattleshock();
    if (numFleeing > 0) {
        SimLog(Verbosity::Narrative, "A total of %d %s from %s fled from battleshock.\n", numFleeing,
               attackingUnit()->name().c_str(),
               PlayerIdToString(m_attackingPlayer).c_str());
    }
    numFleeing = defendingUnit()->applyBattleshock();
    if (numFleeing > 0) {
        SimLog(Verbosity::Narrative, "A total of %d %s from %s fled from battleshock.\n", numFleeing,
               defendingUnit()->name().c_str(),
               PlayerIdToString(m_defendingPlayer).c_str());
    }
}

ManoAMano::~ManoAMano() {
    delete m_rosters[0];
    delete m_rosters[1];
}

PlayerId ManoAMano::getVictor() const {
    // Blue 'tabled' Red
    if (redUnit()->remainingModels() == 0 && blueUnit()->remainingModels() > 0) {
        return PlayerId::Blue;
    }
    // Red 'tabled' Blue
    if (redUnit()->remainingModels() > 0 && blueUnit()->remainingModels() == 0) {
        return PlayerId::Red;
    }
    // Red suffered fewer losses
    if ((float) redUnit()->remainingPoints() / (float) redUnit()->points() >
        (float) blueUnit()->remainingPoints() / (float) blueUnit()->points()) {
        return PlayerId::Red;
    }
    // Blue suffered few losses
    if ((float) redUnit()->remainingPoints() / (float) redUnit()->points() <
        (float) blueUnit()->remainingPoints() / (float) blueUnit()->points()) {
        return PlayerId::Blue;
    }

    // Tie
    return PlayerId::None;
}

static void logUnitStats(const UnitStatistics &stats) {
    Wounds woundsTaken, woundsInflicted;
    stats.totalWoundsInflicted(woundsInflicted);
    stats.totalWoundsTaken(woundsTaken);
    SimLog(Verbosity::Narrative, "\tTotal Movement: %f  Rounds Moved: %d\n", stats.totalMovementDistance(),
           stats.numberOfRoundsMoved());
    SimLog(Verbosity::Narrative, "\tTotal Run Distance: %f  Rounds Ran: %d\n", stats.totalRunDistance(),
           stats.numberOfRoundsRan());
    SimLog(Verbosity::Narrative, "\tTotal Charge Distance: %f  Rounds Charged: %d\n", stats.totalChargeDistance(),
           stats.numberOfRoundsCharged());
    SimLog(Verbosity::Narrative, "\tTotal Enemy Models Slain: %d  Wounds Inflicted: {%d, %d}\n",
           stats.totalEnemyModelsSlain(),
           woundsInflicted.normal, woundsInflicted.mortal);
    SimLog(Verbosity::Narrative, "\tTotal Models Slain: %d  Wounds Taken: {%d, %d}\n", stats.totalModelsSlain(),
           woundsTaken.normal, woundsTaken.mortal);
    SimLog(Verbosity::Narrative, "\tTotal Models Fled: %d\n", stats.totalModelsFled());
}

void ManoAMano::logStatistics() const {
    std::function<void(const TurnRecord &)> turnVistor = [](const TurnRecord &turn) {
        SimLog(Verbosity::Narrative, "\tTurn %d  Player: %s\n", turn.m_round,
               PlayerIdToString(turn.m_playerWithTurn).c_str());
        SimLog(Verbosity::Narrative, "\t\tMoved: %f  Ran: %f Charged: %f\n", turn.m_moved, turn.m_ran, turn.m_charged);
        SimLog(Verbosity::Narrative, "\t\tAttacks Made: %d  Attacks Hit: %d\n", turn.m_attacksMade,
               turn.m_attacksHitting);
        SimLog(Verbosity::Narrative, "\t\tEnemy Slain: %d  Wounds Inflicted: {%d, %d}\n", turn.m_enemyModelsSlain,
               turn.m_woundsInflicted.normal, turn.m_woundsInflicted.mortal);
        SimLog(Verbosity::Narrative, "\t\tSaves Made: %d  Failed: %d\n", turn.m_savesMade, turn.m_savesFailed);
        SimLog(Verbosity::Narrative, "\t\tModel Slain: %d  Wounds Taken: {%d, %d}\n", turn.m_modelsSlain,
               turn.m_woundsTaken.normal, turn.m_woundsTaken.mortal);
    };

    auto redStats = redUnit()->getStatistics();
    SimLog(Verbosity::Narrative, "Red Statistics:\n");
    logUnitStats(redStats);
    if (GetVerbosity() == Verbosity::Narrative)
        redStats.visitTurn(turnVistor);

    auto blueStats = blueUnit()->getStatistics();
    SimLog(Verbosity::Narrative, "Blue Statistics:\n");
    logUnitStats(blueStats);
    if (GetVerbosity() == Verbosity::Narrative)
        blueStats.visitTurn(turnVistor);
}

Unit *ManoAMano::redUnit() {
    auto red = m_rosters[0]->unitBegin();
    return *red;
}

Unit *ManoAMano::blueUnit() {
    auto blue = m_rosters[1]->unitBegin();
    return *blue;
}

Unit *ManoAMano::attackingUnit() {
    if (m_attackingPlayer == PlayerId::Red) {
        return redUnit();
    } else if (m_attackingPlayer == PlayerId::Blue) {
        return blueUnit();
    }
    return nullptr;
}

Unit *ManoAMano::defendingUnit() {
    if (m_defendingPlayer == PlayerId::Red) {
        return redUnit();
    } else if (m_defendingPlayer == PlayerId::Blue) {
        return blueUnit();
    }
    return nullptr;
}

const Unit *ManoAMano::redUnit() const {
    const auto red = m_rosters[0]->unitBegin();
    return *red;
}

const Unit *ManoAMano::blueUnit() const {
    const auto blue = m_rosters[1]->unitBegin();
    return *blue;
}

void ManoAMano::getStatistics(PlayerId which, UnitStatistics &stats) {
    if (which == PlayerId::Red) {
        stats = redUnit()->getStatistics();
    } else if (which == PlayerId::Blue) {
        stats = blueUnit()->getStatistics();
    }
}
