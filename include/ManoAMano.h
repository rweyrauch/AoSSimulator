/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MANOAMANO_H
#define MANOAMANO_H

#include <WarhammerSim.h>
#include <Unit.h>

class Roster;

class ManoAMano
{
public:
    static const int BoardWidth = 30;
    static const int BoardDepth = 24;

    ManoAMano(int numRounds, bool verbose);
    ~ManoAMano();

    void combatants(Unit* red, Unit* blue);

    void start();
    void simulate();
    void next();
    bool done();

    int currentRound() const { return m_round; }
    Phase currentPhase() const { return m_currentPhase; }

    PlayerId getVictor() const;

protected:

    void runInitiativePhase();
    void runHeroPhase();
    void runMovementPhase();
    void runShootingPhase();
    void runChargePhase();
    void runCombatPhase();
    void runBattleshockPhase();

private:

    bool m_verbose = false;
    int m_numRounds = 5;
    Roster* m_rosters[2] = {nullptr, nullptr};
    Unit* m_units[2];

    int m_round = 0;
    bool m_topOfRound = true;
    PlayerId m_attackingUnit = PlayerId::Red;
    PlayerId m_defendingUnit = PlayerId::Blue;
    Phase m_currentPhase = Phase::Hero;
    bool m_isDone = false;

};

#endif // MANOAMANO_H