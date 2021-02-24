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
#include <plog/Log.h>

ManoAMano::ManoAMano(int numRounds, Realm realm) :
        m_numRounds(numRounds) {
    auto board = Board::Instance();
    board->setSize(s_boardWidth, s_boardDepth);
    board->setRealm(realm);
}

void ManoAMano::combatants(std::shared_ptr<Unit> red, std::shared_ptr<Unit> blue) {
    auto board = Board::Instance();


    m_rosters[0] = std::make_shared<Roster>(PlayerId::Red);
    m_rosters[0]->addUnit(red);

    m_rosters[1] = std::make_shared<Roster>(PlayerId::Blue);
    m_rosters[1]->addUnit(blue);

    // +-----------------------+
    // |                       |
    // |                       |
    // | red              blue |
    // |                       |
    // |                       |
    // +-----------------------+

    double redX = board->width() / 10.0;
    double redY = board->depth() / 2.0;

    // left center
    m_initialPos[0] = Math::Point3(redX, redY, 0.0);
    red->deploy(m_initialPos[0], Math::Vector3(1.0, 0.0, 0.0));

    double blueX = board->width() - (board->width() / 10.0);
    double blueY = board->depth() / 2.0;

    // right center
    m_initialPos[1] = Math::Point3(blueX, blueY, 0.0);
    blue->deploy(m_initialPos[1], Math::Vector3(-1.0, 0.0, 0.0));

    board->addRosters(m_rosters[0], m_rosters[1]);

    // Setup a Bad Moon if a player has a Gloomspite Gitz unit.
    if (red->hasKeyword(GLOOMSPITE_GITZ)) {
        GloomspiteGitz::BadMoon::Instance()->setup(GloomspiteGitz::BadMoon::Location::Northwest);
    } else if (blue->hasKeyword(GLOOMSPITE_GITZ)) {
        GloomspiteGitz::BadMoon::Instance()->setup(GloomspiteGitz::BadMoon::Location::Northeast);
    }
}

void ManoAMano::start() {
    redUnit()->deploy(m_initialPos[0], Math::Vector3(1.0, 0.0, 0.0));
    blueUnit()->deploy(m_initialPos[1], Math::Vector3(-1.0, 0.0, 0.0));

    int redRoll = 0;
    int blueRoll = 0;
    while (redRoll == blueRoll) {
        redRoll = Dice::RollD6();
        blueRoll = Dice::RollD6();
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
    PLOG_INFO.printf("Fight State:\n");
    PLOG_INFO.printf("\tRound: %d of %d.  Top of round: %d", m_round, m_numRounds, m_topOfRound);
    PLOG_INFO.printf("\tPhase: %s\n", PhaseToString(m_currentPhase).c_str());
    PLOG_INFO.printf("\tCurrent Unit: %s\n", PlayerIdToString(m_attackingPlayer).c_str());

    // run the simulation for the current state
    switch (m_currentPhase) {
        case Phase::Deployment:
            break;
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
        case Phase::Deployment:
            m_currentPhase = Phase::Initiative;
            break;
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
                PLOG_INFO.printf("At the end of round %d...\n", m_round);
                PLOG_INFO.printf("\tTeam %s has %d remaining models with %d wounds remaining.\n",
                       PlayerIdToString(PlayerId::Red).c_str(), redUnit()->remainingModels(),
                       redUnit()->remainingWounds());
                PLOG_INFO.printf("\tTeam %s has %d remaining models with %d wounds remaining.\n",
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

bool ManoAMano::done() const {
    return m_isDone;
}

void ManoAMano::runInitiativePhase() {
    // Roll D6 for each player, highest goes first.
    auto p1 = Dice::RollD6();
    auto p2 = Dice::RollD6();
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

    PLOG_INFO.printf("Player %s wins initiative. Dice rolls: Red: %d  Blue: %d.\n",
           PlayerIdToString(m_attackingPlayer).c_str(), p1, p2);
}

void ManoAMano::runHeroPhase() {
    PLOG_INFO.printf("Starting player %s hero phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doHeroPhase();
}

void ManoAMano::runMovementPhase() {
    PLOG_INFO.printf("Starting player %s movement phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doMovementPhase();

    if (attackingUnit()->ran()) {
        PLOG_INFO.printf("%s:%s ran.\n", PlayerIdToString(m_attackingPlayer).c_str(),
               attackingUnit()->name().c_str());
    }
}

void ManoAMano::runShootingPhase() {
    PLOG_INFO.printf("Starting player %s shooting phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    // Think...
    m_rosters[(int) m_attackingPlayer]->doShootingPhase();

    // Act...
    int numSlain = 0;
    auto totalDamage = attackingUnit()->shoot(numSlain);

    PLOG_INFO.printf("%s:%s did %d shooting damage to %s:%s slaying %d models.\n",
           PlayerIdToString(m_attackingPlayer).c_str(), attackingUnit()->name().c_str(),
           (totalDamage.normal + totalDamage.mortal),
           PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str(), numSlain);
}

void ManoAMano::runChargePhase() {
    PLOG_INFO.printf("Starting player %s charge phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doChargePhase();

    if (attackingUnit()->charged()) {
        PLOG_INFO.printf("%s:%s charged %s:%s.\n", PlayerIdToString(m_attackingPlayer).c_str(),
               attackingUnit()->name().c_str(),
               PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str());
    }
}

void ManoAMano::runCombatPhase() {
    PLOG_INFO.printf("Starting player %s combat phase.\n", PlayerIdToString(m_attackingPlayer).c_str());

    // Think.
    m_rosters[(int) m_attackingPlayer]->doCombatPhase();

    //assert(attackingUnit()->meleeTarget() == defendingUnit());

    int numSlain = 0;
    auto totalDamage = attackingUnit()->fight(m_attackingPlayer, numSlain);

    PLOG_INFO.printf("%s:%s did %d damage to %s:%s slaying %d models in the combat phase.\n",
           PlayerIdToString(m_attackingPlayer).c_str(),
           attackingUnit()->name().c_str(), (totalDamage.normal + totalDamage.mortal),
           PlayerIdToString(m_defendingPlayer).c_str(), defendingUnit()->name().c_str(),
           numSlain);

    numSlain = 0;
    totalDamage = defendingUnit()->fight(-1, attackingUnit(), numSlain);

    PLOG_INFO.printf("%s:%s did %d damage to %s:%s slaying %d models in the counter attack.\n",
           PlayerIdToString(m_defendingPlayer).c_str(),
           defendingUnit()->name().c_str(), (totalDamage.normal + totalDamage.mortal),
           PlayerIdToString(m_attackingPlayer).c_str(), attackingUnit()->name().c_str(),
           numSlain);
}

void ManoAMano::runBattleshockPhase() {
    PLOG_INFO.printf("Starting player %s battleshock phase.\n",
           PlayerIdToString(m_attackingPlayer).c_str());

    m_rosters[(int) m_attackingPlayer]->doBattleshockPhase();
    m_rosters[(int) m_defendingPlayer]->doBattleshockPhase();
}

ManoAMano::~ManoAMano() {
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
    if ((double) redUnit()->remainingPoints() / (double) redUnit()->points() >
        (double) blueUnit()->remainingPoints() / (double) blueUnit()->points()) {
        return PlayerId::Red;
    }
    // Blue suffered few losses
    if ((double) redUnit()->remainingPoints() / (double) redUnit()->points() <
        (double) blueUnit()->remainingPoints() / (double) blueUnit()->points()) {
        return PlayerId::Blue;
    }

    // Tie
    return PlayerId::None;
}

static void logUnitStats(const UnitStatistics &stats) {
    Wounds woundsTaken, woundsInflicted;
    stats.totalWoundsInflicted(woundsInflicted);
    stats.totalWoundsTaken(woundsTaken);
    PLOG_INFO.printf("\tTotal Movement: %f  Rounds Moved: %d\n", stats.totalMovementDistance(),
           stats.numberOfRoundsMoved());
    PLOG_INFO.printf("\tTotal Run Distance: %f  Rounds Ran: %d\n", stats.totalRunDistance(),
           stats.numberOfRoundsRan());
    PLOG_INFO.printf("\tTotal Charge Distance: %f  Rounds Charged: %d\n", stats.totalChargeDistance(),
           stats.numberOfRoundsCharged());
    PLOG_INFO.printf("\tTotal Enemy Models Slain: %d  Wounds Inflicted: {%d, %d}\n",
           stats.totalEnemyModelsSlain(),
           woundsInflicted.normal, woundsInflicted.mortal);
    PLOG_INFO.printf("\tTotal Models Slain: %d  Wounds Taken: {%d, %d}\n", stats.totalModelsSlain(),
           woundsTaken.normal, woundsTaken.mortal);
    PLOG_INFO.printf("\tTotal Models Fled: %d\n", stats.totalModelsFled());
}

void ManoAMano::logStatistics() const {
    std::function<void(const TurnRecord &)> turnVistor = [](const TurnRecord &turn) {
        PLOG_INFO.printf("\tTurn %d  Player: %s\n", turn.m_round,
               PlayerIdToString(turn.m_playerWithTurn).c_str());
        PLOG_INFO.printf("\t\tMoved: %f  Ran: %f Charged: %f\n", turn.m_moved, turn.m_ran, turn.m_charged);
        PLOG_INFO.printf("\t\tAttacks Made: %d  Attacks Hit: %d\n", turn.m_attacksMade,
               turn.m_attacksHitting);
        PLOG_INFO.printf("\t\tEnemy Slain: %d  Wounds Inflicted: {%d, %d}\n", turn.m_enemyModelsSlain,
               turn.m_woundsInflicted.normal, turn.m_woundsInflicted.mortal);
        PLOG_INFO.printf("\t\tSaves Made: %d  Failed: %d\n", turn.m_savesMade, turn.m_savesFailed);
        PLOG_INFO.printf("\t\tModel Slain: %d  Wounds Taken: {%d, %d}\n", turn.m_modelsSlain,
               turn.m_woundsTaken.normal, turn.m_woundsTaken.mortal);
    };

    auto redStats = redUnit()->getStatistics();
    PLOG_INFO.printf("Red Statistics:\n");
    logUnitStats(redStats);
    redStats.visitTurn(turnVistor);

    auto blueStats = blueUnit()->getStatistics();
    PLOG_INFO.printf("Blue Statistics:\n");
    logUnitStats(blueStats);
    blueStats.visitTurn(turnVistor);
}

Unit *ManoAMano::redUnit() {
    auto red = m_rosters[0]->unitBegin();
    return red->get();
}

Unit *ManoAMano::blueUnit() {
    auto blue = m_rosters[1]->unitBegin();
    return blue->get();
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
    return red->get();
}

const Unit *ManoAMano::blueUnit() const {
    const auto blue = m_rosters[1]->unitBegin();
    return blue->get();
}

void ManoAMano::getStatistics(PlayerId which, UnitStatistics &stats) {
    if (which == PlayerId::Red) {
        stats = redUnit()->getStatistics();
    } else if (which == PlayerId::Blue) {
        stats = blueUnit()->getStatistics();
    }
}

void ManoAMano::clearStatistics() {
    redUnit()->clearStatistics();
    blueUnit()->clearStatistics();
}
