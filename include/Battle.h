/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <AgeOfSigmarSim.h>
#include <Player.h>

class Battle {
public:
    Battle() = default;

    void addPlayers(Player *player1, Player *player2);

    void setNumRounds(int rounds) { m_numRounds = rounds; }

    void deployment();

    void start(PlayerId firstPlayer);

    void simulate();

    void next();

    [[nodiscard]] bool done() const;

    [[nodiscard]] int currentRound() const { return m_round; }

    [[nodiscard]] PlayerId activePlayer() const { return m_currentPlayer; }

    [[nodiscard]] Phase currentPhase() const { return m_currentPhase; }

protected:

    void runInitiativePhase();

    void runHeroPhase();

    void runMovementPhase();

    void runShootingPhase();

    void runChargePhase();

    void runCombatPhase();

    void runBattleshockPhase();

private:
    Player *m_players[2] = {nullptr, nullptr};
    int m_numRounds = 5;

    int m_round = 0;
    bool m_topOfRound = true;
    PlayerId m_currentPlayer = PlayerId::Red;
    Phase m_currentPhase = Phase::Hero;
    bool m_isDone = false;
};

