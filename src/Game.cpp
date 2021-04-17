/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "Game.h"

void Game::setRoster(PlayerId player, const std::shared_ptr<Roster>& roster) {
    if (player == PlayerId::Red) {
        m_rosters[0] = roster;
    }
    else if (player == PlayerId::Blue) {
        m_rosters[1] = roster;
    }
}

void Game::setBoard(const std::shared_ptr<Board>& board) {
    m_board = board;
}

void Game::start() {

    // Initiative
    int redRoll = 0;
    int blueRoll = 0;
    while (redRoll == blueRoll) {
        redRoll = Dice::RollD6();
        blueRoll = Dice::RollD6();
    }
    if (redRoll > blueRoll) {
        m_attackingPlayer = PlayerId::Red;
    } else {
        m_attackingPlayer = PlayerId::Blue;
    }
    if (m_attackingPlayer == PlayerId::Red) {
        m_defendingPlayer = PlayerId::Blue;
    } else {
        m_defendingPlayer = PlayerId::Red;
    }
    m_round = 1;

    // Place objectives
    placeObjectives();

    // Deploy rosters
    deploy(m_defendingPlayer);
    deploy(m_attackingPlayer);

    beginBattleRound();
    beginTurn(m_attackingPlayer);
}

void Game::simulate() {
    // run the simulation for the current state
    switch (m_currentPhase) {
        case GamePhase::Deployment:
        case GamePhase::Initiative:
            break;
        case GamePhase::Hero:
            hero(m_attackingPlayer);
            break;
        case GamePhase::Movement:
            movement(m_attackingPlayer);
            break;
        case GamePhase::Shooting:
            shooting(m_attackingPlayer);
            break;
        case GamePhase::Charge:
            charge(m_attackingPlayer);
            break;
        case GamePhase::Combat:
            combat(m_attackingPlayer);
            break;
        case GamePhase::Battleshock:
            battleshock(m_attackingPlayer);
            break;
    }
}

void Game::next() {

    // advance fight state machine
    switch (m_currentPhase) {
        case GamePhase::Deployment:
        case GamePhase::Initiative:
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
                // Next unit's turn
                m_topOfRound = false;

                endTurn(m_attackingPlayer);
                std::swap(m_attackingPlayer, m_defendingPlayer);
                beginTurn(m_attackingPlayer);

                m_currentPhase = GamePhase::Hero;
            } else {

                endTurn(m_attackingPlayer);
                endBattleRound();

                // End of round.
                m_topOfRound = true;
                m_round++;

                // End of battle.
                if (m_round > m_numRounds) {
                    m_isDone = true;
                } else {
                    beginBattleRound();
                    beginTurn(m_attackingPlayer);
                }
            }
            break;
    }
}

bool Game::done() const {
    return m_isDone;
}

void Game::reset() {

}

void Game::beginBattleRound() {
    m_beginRound(m_round);
}

void Game::endBattleRound() {
    m_endRound(m_round);
}

void Game::beginTurn(PlayerId player) {
    m_beginTurn(player, m_round);
}

void Game::endTurn(PlayerId player) {
    m_endTurn(player, m_round);
}

void Game::hero(PlayerId player) {
    m_startHero(player);
    m_heroPhase.run(player, m_board.get());
    m_endHero(player);
}

void Game::movement(PlayerId player) {
    m_startMovement(player);
    m_movementPhase.run(player, m_board.get());
    m_endMovement(player);
}

void Game::shooting(PlayerId player) {
    m_startShooting(player);
    m_shootingPhase.run(player, m_board.get());
    m_endShooting(player);
}

void Game::charge(PlayerId player) {
    m_startCharge(player);
    m_chargePhase.run(player, m_board.get());
    m_endCharge(player);
}

void Game::combat(PlayerId player) {
    m_startCombat(player);
    m_combatPhase.run(player, m_board.get());
    m_endCombat(player);
}

void Game::battleshock(PlayerId player) {
    m_startBattleshock(player);
    m_battleshockPhase.run(player, m_board.get());
    m_endBattleshock(player);
}

void Game::placeObjectives() {

}

void Game::deploy(PlayerId player) {

}
