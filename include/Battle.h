/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_BATTLE_H
#define WARHAMMERSIM_BATTLE_H

#include <WarhammerSim.h>
#include <Player.h>
#include <Board.h>

enum class Phase
{
    Initiative,
    Hero,
    Movement,
    Shooting,
    Charge,
    Combat,
    Battleshock
};

class Battle
{
public:
    Battle() = default;

    void addPlayers(const Player& player1, const Player& player2);
    void setBoard(const Board& board) { m_board = board; }
    void setNumRounds(int rounds) { m_numRounds = rounds; }

    void deployment();

    void start(PlayerId firstPlayer);
    void simulate();
    void next();
    bool done();

    int currentRound() const { return m_round; }
    PlayerId activePlayer() const { return m_currentPlayer; }
    Phase currentPhase() const { return m_currentPhase; }

protected:

    void runInitiativePhase();
    void runHeroPhase();
    void runMovementPhase();
    void runShootingPhase();
    void runChargePhase();
    void runCombatPhase();
    void runBattleshockPhase();

    std::string phaseToString(Phase phase) const;
    std::string playerIdToString(PlayerId id) const;

private:
    Player m_players[2];
    Board m_board;
    int m_numRounds = 5;

    int m_round = 0;
    bool m_topOfRound = true;
    PlayerId m_currentPlayer = PlayerId::One;
    Phase m_currentPhase = Phase::Hero;
    bool m_isDone = false;
};


#endif //WARHAMMERSIM_BATTLE_H
