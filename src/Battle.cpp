/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Dice.h>
#include <Battle.h>
#include <Board.h>

void Battle::start(PlayerId firstPlayer) {
    m_topOfRound = true;
    m_currentPlayer = firstPlayer;
    m_currentPhase = GamePhase::Hero;
    m_round = 1;
}

void Battle::next() {
    // advance battle state machine
    switch (m_currentPhase) {
        case GamePhase::Deployment:
            m_currentPhase = GamePhase::Initiative;
            break;
        case GamePhase::Initiative:
            m_currentPhase = GamePhase::Hero;
            break;
        case GamePhase::Hero:
            m_currentPhase = GamePhase::Movement;
            break;
        case GamePhase::Movement:
            m_currentPhase = GamePhase::Shooting;
            break;
        case GamePhase::Shooting:
            m_currentPhase = GamePhase::Charge;
            break;
        case GamePhase::Charge:
            m_currentPhase = GamePhase::Combat;
            break;
        case GamePhase::Combat:
            m_currentPhase = GamePhase::Battleshock;
            break;
        case GamePhase::Battleshock:
            if (m_topOfRound) {

                m_players[(int) m_currentPlayer]->endTurn(m_round);

                // Next players turn
                m_topOfRound = false;
                if (m_currentPlayer == PlayerId::Red) {
                    m_currentPlayer = PlayerId::Blue;
                } else {
                    m_currentPlayer = PlayerId::Red;
                }

                m_players[(int) m_currentPlayer]->beginTurn(m_round, m_currentPlayer);

                m_currentPhase = GamePhase::Hero;
            } else {

                m_players[(int) m_currentPlayer]->endTurn(m_round);

                // End of round.
                m_currentPhase = GamePhase::Initiative;
                m_topOfRound = true;
                m_round++;

                // End of battle.
                if (m_round > m_numRounds) {
                    m_isDone = true;
                }
            }
            break;
    }
}

bool Battle::done() const {
    return m_isDone;
}

void Battle::simulate() {
    PLOG_INFO.printf("Battle State:");
    PLOG_INFO.printf("\tRound: %d of %d.  Top of round: %d", m_round, m_numRounds, m_topOfRound);
    PLOG_INFO.printf("\tPhase: %s", PhaseToString(m_currentPhase).c_str());
    PLOG_INFO.printf("\tCurrent Player: %s", PlayerIdToString(m_currentPlayer).c_str());

    // run the simulation for the current state
    switch (m_currentPhase) {
        case GamePhase::Deployment:
            break;
        case GamePhase::Initiative:
            runInitiativePhase();
            break;
        case GamePhase::Hero:
            runHeroPhase();
            break;
        case GamePhase::Movement:
            runMovementPhase();
            break;
        case GamePhase::Shooting:
            runShootingPhase();
            break;
        case GamePhase::Charge:
            runChargePhase();
            break;
        case GamePhase::Combat:
            runCombatPhase();
            break;
        case GamePhase::Battleshock:
            runBattleshockPhase();
            break;
    }
}

void Battle::addPlayers(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2) {
    m_players[0] = player1;
    m_players[1] = player2;
}

void Battle::runInitiativePhase() {
    // Roll D6 for each player, highest goes first.
    auto p1 = Dice::RollD6();
    auto p2 = Dice::RollD6();
    if (p1 == p2) {
        // Ties go to the player that went first in the previous round.
        m_currentPlayer = (m_currentPlayer == PlayerId::Red) ? PlayerId::Blue : PlayerId::Red;
    } else if (p1 > p2) {
        m_currentPlayer = PlayerId::Red;
    } else {
        m_currentPlayer = PlayerId::Blue;
    }

    const auto playerIdx = (int) m_currentPlayer;
    m_players[playerIdx]->beginTurn(m_round, m_currentPlayer);

    PLOG_INFO.printf("Player %s wins initiative.  P1: %d P2: %d", PlayerIdToString(m_currentPlayer).c_str(),
                     p1, p2);
}

void Battle::runHeroPhase() {
    auto unit = m_players[(int) m_currentPlayer]->startPhase(GamePhase::Hero);
    while (unit) {
        unit->hero(m_currentPlayer);

        unit = m_players[(int) m_currentPlayer]->advancePhase();
    }
    m_players[(int) m_currentPlayer]->endPhase();
}

void Battle::runMovementPhase() {
    auto unit = m_players[(int) m_currentPlayer]->startPhase(GamePhase::Movement);
    while (unit) {
        unit->movement(m_currentPlayer);

        unit = m_players[(int) m_currentPlayer]->advancePhase();
    }
    m_players[(int) m_currentPlayer]->endPhase();
}

void Battle::runShootingPhase() {
    auto unit = m_players[(int) m_currentPlayer]->startPhase(GamePhase::Shooting);
    while (unit) {
        unit->shooting(m_currentPlayer);

        int numSlain = 0;
        auto totalDamage = unit->shoot(m_currentPlayer, numSlain);
        if (unit->shootingTarget() && !totalDamage.zero()) {
            PLOG_INFO.printf("%s:%s did %d shooting damage to %s:%s slaying %d models.",
                             PlayerIdToString(m_currentPlayer).c_str(),
                             unit->name().c_str(),
                             (totalDamage.normal + totalDamage.mortal),
                             PlayerIdToString(GetEnemyId(m_currentPlayer)).c_str(),
                             unit->shootingTarget()->name().c_str(), numSlain);
        }
        unit = m_players[(int) m_currentPlayer]->advancePhase();
    }
    m_players[(int) m_currentPlayer]->endPhase();
}

void Battle::runChargePhase() {
    auto unit = m_players[(int) m_currentPlayer]->startPhase(GamePhase::Charge);
    while (unit) {
        unit->charge(m_currentPlayer);

        if (unit->charged()) {
            PLOG_INFO.printf("%s:%s charged.",
                             PlayerIdToString(m_currentPlayer).c_str(),
                             unit->name().c_str());
        }

        unit = m_players[(int) m_currentPlayer]->advancePhase();
    }
    m_players[(int) m_currentPlayer]->endPhase();
}

void Battle::runCombatPhase() {

    // Combine/sort activated units
    // TODO: Some abilities move enemy units to attack first
    auto unit = m_players[(int) m_currentPlayer]->startPhase(GamePhase::Combat);
    const auto defendingPlayer = GetEnemyId(m_currentPlayer);
    auto eunit = m_players[(int) defendingPlayer]->startPhase(GamePhase::Combat);
    while (unit || eunit) {
        if (unit) {
            unit->combat(m_currentPlayer);

            int numSlain = 0;
            auto totalDamage = unit->fight(m_currentPlayer, numSlain);

            if (unit->meleeTarget() && !totalDamage.zero()) {
                PLOG_INFO.printf("%s: %s did %d damage to %s:%s slaying %d models in the combat phase.",
                                 PlayerIdToString(m_currentPlayer).c_str(),
                                 unit->name().c_str(), (totalDamage.normal + totalDamage.mortal),
                                 PlayerIdToString(defendingPlayer).c_str(), unit->meleeTarget()->name().c_str(),
                                 numSlain);
            }
        }
        if (eunit) {
            eunit->combat(m_currentPlayer);

            int numSlain = 0;
            auto totalDamage = eunit->fight(m_currentPlayer, numSlain);

            if (eunit->meleeTarget() && !totalDamage.zero()) {
                PLOG_INFO.printf("%s: %s did %d damage to %s:%s slaying %d models in the combat phase.",
                                 PlayerIdToString(defendingPlayer).c_str(),
                                 eunit->name().c_str(), (totalDamage.normal + totalDamage.mortal),
                                 PlayerIdToString(m_currentPlayer).c_str(), eunit->meleeTarget()->name().c_str(),
                                 numSlain);
            }
        }

        // Advance to next units
        unit = m_players[(int) m_currentPlayer]->advancePhase();
        eunit = m_players[(int) GetEnemyId(m_currentPlayer)]->advancePhase();
    }

    m_players[(int) m_currentPlayer]->endPhase();
    m_players[(int) GetEnemyId(m_currentPlayer)]->endPhase();
}

void Battle::runBattleshockPhase() {
    auto unit = m_players[(int) m_currentPlayer]->startPhase(GamePhase::Battleshock);
    while (unit) {
        unit->battleshock(m_currentPlayer);

        unit = m_players[(int) m_currentPlayer]->advancePhase();
    }
    unit = m_players[(int) GetEnemyId(m_currentPlayer)]->startPhase(GamePhase::Battleshock);
    while (unit) {
        unit->battleshock(m_currentPlayer);

        unit = m_players[(int) GetEnemyId(m_currentPlayer)]->advancePhase();
    }

    m_players[(int) m_currentPlayer]->endPhase();
    m_players[(int) GetEnemyId(m_currentPlayer)]->endPhase();
}

void Battle::deployment() {

    auto board = Board::Instance();

    PlayerId firstPlayer = PlayerId::None;
    // initiative to select player deploying first
    while (firstPlayer == PlayerId::None) {
        auto redRoll = Dice::RollD6();
        auto blueRoll = Dice::RollD6();
        if (redRoll > blueRoll) firstPlayer = PlayerId::Red;
        else if (blueRoll > redRoll) firstPlayer = PlayerId::Blue;
    }
    PlayerId secondPlayer = GetEnemyId(firstPlayer);

    // +-----------------------+
    // |                       |
    // |                       |
    // | first          second |
    // |                       |
    // |                       |
    // +-----------------------+

    double fX = board->width() / 10.0;
    double fY = board->depth() / 2.0;

    // left center
    auto fPos = Math::Point3(fX, fY, 0.0);
    auto fOrientation = Math::Vector3(1, 0, 0);

    double sX = board->width() - (board->width() / 10.0);
    double sY = board->depth() / 2.0;

    // right center
    auto sPos = Math::Point3(sX, sY, 0.0);
    auto sOrientation = Math::Vector3(-1.0, 0.0, 0.0);

    // loop until all units have been deployed
    auto unit = m_players[(int) firstPlayer]->startPhase(GamePhase::Deployment);
    auto eunit = m_players[(int) secondPlayer]->startPhase(GamePhase::Deployment);
    while (unit || eunit) {
        if (unit) {
            unit->deploy(fPos, fOrientation);
        }
        if (eunit) {
            eunit->deploy(sPos, sOrientation);
        }

        // Advance to next units
        unit = m_players[(int) firstPlayer]->advancePhase();
        eunit = m_players[(int) secondPlayer]->advancePhase();
    }
    m_players[(int) firstPlayer]->endPhase();
    m_players[(int) secondPlayer]->endPhase();
}
