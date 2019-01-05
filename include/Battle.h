/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_BATTLE_H
#define WARHAMMERSIM_BATTLE_H

#include "Player.h"
#include "Board.h"

enum class Phase
{
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

    void players(const Player& player1, const Player& player2);

    void start();
    void run();
    void next();
    bool done();

private:
    Player m_players[2];
    Board m_board;

    int m_round = 0;
    int m_firstPlayer = 0;
    int m_currentPlayer = 0;
    Phase m_currentPhase = Phase::Hero;
};


#endif //WARHAMMERSIM_BATTLE_H
