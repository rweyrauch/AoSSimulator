/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MANOAMANO_H
#define MANOAMANO_H

#include <AgeOfSigmarSim.h>
#include <Unit.h>

class Roster;

class ManoAMano
{
public:
    static const int BoardWidth = 36;
    static const int BoardDepth = 48;

    explicit ManoAMano(int numRounds, Realm realm = Azyr);
    ~ManoAMano();

    void combatants(Unit* red, Unit* blue);

    void start();
    void simulate();
    void next();
    bool done();

    int currentRound() const { return m_round; }
    Phase currentPhase() const { return m_currentPhase; }

    void logStatistics() const;
    PlayerId getVictor() const;

protected:

    void runInitiativePhase();
    void runHeroPhase();
    void runMovementPhase();
    void runShootingPhase();
    void runChargePhase();
    void runCombatPhase();
    void runBattleshockPhase();

    Unit* redUnit();
    Unit* blueUnit();
    const Unit* redUnit() const;
    const Unit* blueUnit() const;
    Unit* attackingUnit();
    Unit* defendingUnit();

private:

    int m_numRounds = 5;
    Roster* m_rosters[2] = {nullptr, nullptr};
    Math::Point3 m_initialPos[2];

    int m_cpAvailable[2] = {0, 0};

    int m_round = 0;
    bool m_topOfRound = true;
    PlayerId m_attackingPlayer = PlayerId::Red;
    PlayerId m_defendingPlayer = PlayerId::Blue;
    Phase m_currentPhase = Phase::Hero;
    bool m_isDone = false;

};

#endif // MANOAMANO_H