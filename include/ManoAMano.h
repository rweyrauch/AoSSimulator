/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <AgeOfSigmarSim.h>
#include <Unit.h>

class Roster;

class ManoAMano {
public:
    static const int s_boardWidth = 48;
    static const int s_boardDepth = 36;

    explicit ManoAMano(int numRounds, Realm realm = Azyr);

    ~ManoAMano();

    void combatants(Unit *red, Unit *blue);

    void start();

    void simulate();

    void next();

    [[nodiscard]] bool done() const;

    [[nodiscard]] int currentRound() const { return m_round; }

    [[nodiscard]] Phase currentPhase() const { return m_currentPhase; }

    void logStatistics() const;

    void getStatistics(PlayerId which, UnitStatistics &stats);

    [[nodiscard]] PlayerId getVictor() const;

    [[nodiscard]] double getInitialRedX() const { return m_initialPos[0].x; }

    [[nodiscard]] double getInitialRedY() const { return m_initialPos[0].y; }

    [[nodiscard]] double getInitialBlueX() const { return m_initialPos[1].x; }

    [[nodiscard]] double getInitialBlueY() const { return m_initialPos[1].y; }

    [[nodiscard]] double getBoardWidth() const { return s_boardWidth; }

    [[nodiscard]] double getBoardDepth() const { return s_boardDepth; }

protected:

    void runInitiativePhase();

    void runHeroPhase();

    void runMovementPhase();

    void runShootingPhase();

    void runChargePhase();

    void runCombatPhase();

    void runBattleshockPhase();

    Unit *redUnit();

    Unit *blueUnit();

    [[nodiscard]] const Unit *redUnit() const;

    [[nodiscard]] const Unit *blueUnit() const;

    Unit *attackingUnit();

    Unit *defendingUnit();

private:

    int m_numRounds = 5;
    Roster *m_rosters[2] = {nullptr, nullptr};
    Math::Point3 m_initialPos[2];

    int m_cpAvailable[2] = {0, 0};

    int m_round = 0;
    bool m_topOfRound = true;
    PlayerId m_attackingPlayer = PlayerId::Red;
    PlayerId m_defendingPlayer = PlayerId::Blue;
    Phase m_currentPhase = Phase::Hero;
    bool m_isDone = false;

};
