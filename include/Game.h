/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <algorithm>

#include "AgeOfSigmarSim.h"
#include "lsignal.h"

#include "Hero.h"
#include "Movement.h"
#include "Shooting.h"
#include "Charge.h"
#include "Combat.h"
#include "Battleshock.h"

class Roster;
class Board;

class Game {

public:
    Game() = default;
    ~Game() = default;

    // Setup functions
    void setRoster(PlayerId player, const std::shared_ptr<Roster>& roster);
    void setBoard(const std::shared_ptr<Board>& board);

    // Simulation controls
    void start();
    void simulate();
    void next();

    // Simulation state
    bool done() const;
    int currentRound() const { return m_round; }

    // Reset
    void reset();

protected:

    // Battle round functions
    void beginBattleRound();
    void endBattleRound();

    void beginTurn(PlayerId player);
    void endTurn(PlayerId player);

    // Phase functions
    void hero(PlayerId player);
    void movement(PlayerId player);
    void shooting(PlayerId player);
    void charge(PlayerId player);
    void combat(PlayerId player);
    void battleshock(PlayerId player);

    void placeObjectives();
    void deploy(PlayerId player);

protected:

    int m_numRounds = 5;
    int m_round = 0;
    bool m_topOfRound = true;
    GamePhase m_currentPhase = GamePhase::Hero;
    bool m_isDone = false;

    PlayerId m_attackingPlayer = PlayerId::Red;
    PlayerId m_defendingPlayer = PlayerId::Blue;

    // Player rosters
    std::shared_ptr<Roster> m_rosters[2] = {nullptr, nullptr};

    std::shared_ptr<Board> m_board = nullptr;

    // Phases
    HeroPhase m_heroPhase;
    MovementPhase m_movementPhase;
    ShootingPhase m_shootingPhase;
    ChargePhase m_chargePhase;
    CombatPhase m_combatPhase;
    BattleshockPhase m_battleshockPhase;

    // Events
    lsignal::signal<void(int)> m_beginRound;
    lsignal::signal<void(int)> m_endRound;
    lsignal::signal<void(PlayerId, int)> m_beginTurn;
    lsignal::signal<void(PlayerId, int)> m_endTurn;

    lsignal::signal<void(PlayerId)> m_startHero;
    lsignal::signal<void(PlayerId)> m_endHero;
    lsignal::signal<void(PlayerId)> m_startMovement;
    lsignal::signal<void(PlayerId)> m_endMovement;
    lsignal::signal<void(PlayerId)> m_startShooting;
    lsignal::signal<void(PlayerId)> m_endShooting;
    lsignal::signal<void(PlayerId)> m_startCharge;
    lsignal::signal<void(PlayerId)> m_endCharge;
    lsignal::signal<void(PlayerId)> m_startCombat;
    lsignal::signal<void(PlayerId)> m_endCombat;
    lsignal::signal<void(PlayerId)> m_startBattleshock;
    lsignal::signal<void(PlayerId)> m_endBattleshock;

};

