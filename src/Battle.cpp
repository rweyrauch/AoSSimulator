/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Dice.h>
#include <Battle.h>

void Battle::start(PlayerId firstPlayer) {
    m_topOfRound = true;
    m_currentPlayer = firstPlayer;
    m_currentPhase = Phase::Hero;
    m_round = 1;
}

void Battle::next() {
    // advance battle state machine
    switch (m_currentPhase) {
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
                // Next players turn
                m_topOfRound = false;
                if (m_currentPlayer == PlayerId::Red) {
                    m_currentPlayer = PlayerId::Blue;
                } else {
                    m_currentPlayer = PlayerId::Red;
                }

                const auto playerIdx = (int) m_currentPlayer;
                m_players[playerIdx]->beginTurn(m_round, m_currentPlayer);

                m_currentPhase = Phase::Hero;
            } else {
                // End of round.
                m_currentPhase = Phase::Initiative;
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
    SimLog(Verbosity::Narrative, "Battle State:\n");
    SimLog(Verbosity::Narrative, "\tRound: %d of %d.  Top of round: %d\n", m_round, m_numRounds, m_topOfRound);
    SimLog(Verbosity::Narrative, "\tPhase: %s\n", PhaseToString(m_currentPhase).c_str());
    SimLog(Verbosity::Narrative, "\tCurrent Player: %s\n", PlayerIdToString(m_currentPlayer).c_str());

    // run the simulation for the current state
    switch (m_currentPhase) {
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

void Battle::addPlayers(Player *player1, Player *player2) {
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

    SimLog(Verbosity::Narrative, "Player %s wins initiative.  P1: %d P2: %d", PlayerIdToString(m_currentPlayer).c_str(),
           p1, p2);
}

void Battle::runHeroPhase() {
    auto unit = m_players[m_currentPlayer]->startPhase(Phase::Hero);
    while (unit) {
        unit->hero(m_currentPlayer);

        unit = m_players[m_currentPlayer]->advancePhase();
    }
    m_players[m_currentPlayer]->endPhase();
}

void Battle::runMovementPhase() {
    auto unit = m_players[m_currentPlayer]->startPhase(Phase::Movement);
    while (unit) {
        unit->movement(m_currentPlayer);

        unit = m_players[m_currentPlayer]->advancePhase();
    }
    m_players[m_currentPlayer]->endPhase();
}

void Battle::runShootingPhase() {
    auto unit = m_players[m_currentPlayer]->startPhase(Phase::Shooting);
    while (unit) {
        unit->shooting(m_currentPlayer);

        int numSlain = 0;
        auto totalDamage = unit->shoot(numSlain);
        SimLog(Verbosity::Narrative, "%s:%s did %d shooting damage to %s:%s slaying %d models.\n",
                PlayerIdToString(m_currentPlayer).c_str(),
                unit->name().c_str(),
                (totalDamage.normal + totalDamage.mortal),
                PlayerIdToString(GetEnemyId(m_currentPlayer)).c_str(),
                unit->meleeTarget()->name().c_str(), numSlain);

        unit = m_players[m_currentPlayer]->advancePhase();
    }
    m_players[m_currentPlayer]->endPhase();
}

void Battle::runChargePhase() {
    auto unit = m_players[m_currentPlayer]->startPhase(Phase::Charge);
    while (unit) {
        unit->charge(m_currentPlayer);

        if (unit->charged()) {
            SimLog(Verbosity::Narrative, "%s:%s charged %s:%s.\n",
                    PlayerIdToString(m_currentPlayer).c_str(),
                    unit->name().c_str(),
                    PlayerIdToString(GetEnemyId(m_currentPlayer)).c_str(),
                    unit->meleeTarget()->name().c_str());
        }

        unit = m_players[m_currentPlayer]->advancePhase();
    }
    m_players[m_currentPlayer]->endPhase();
}

void Battle::runCombatPhase() {

    // Combine/sort activated units
    // TODO: Some abilities move enemy units to attack first
    auto unit = m_players[m_currentPlayer]->startPhase(Phase::Combat);
    const auto defendingPlayer = GetEnemyId(m_currentPlayer);
    auto eunit = m_players[defendingPlayer]->startPhase(Phase::Combat);
    while (unit || eunit) {
        if (unit) {
            unit->combat(m_currentPlayer);

            int numSlain = 0;
            auto totalDamage = unit->fight(m_currentPlayer, numSlain);

            SimLog(Verbosity::Narrative, "%s:%s did %d damage to %s:%s slaying %d models in the combat phase.\n",
                   PlayerIdToString(m_currentPlayer).c_str(),
                   unit->name().c_str(), (totalDamage.normal + totalDamage.mortal),
                   PlayerIdToString(defendingPlayer).c_str(), unit->meleeTarget()->name().c_str(),
                   numSlain);
        }
        if (eunit) {
            eunit->combat(m_currentPlayer);

            int numSlain = 0;
            auto totalDamage = eunit->fight(m_currentPlayer, numSlain);

            SimLog(Verbosity::Narrative, "%s:%s did %d damage to %s:%s slaying %d models in the combat phase.\n",
                   PlayerIdToString(defendingPlayer).c_str(),
                   eunit->name().c_str(), (totalDamage.normal + totalDamage.mortal),
                   PlayerIdToString(m_currentPlayer).c_str(), eunit->meleeTarget()->name().c_str(),
                   numSlain);
        }

        // Advance to next units
        unit = m_players[m_currentPlayer]->advancePhase();
        eunit = m_players[GetEnemyId(m_currentPlayer)]->advancePhase();
    }

    m_players[m_currentPlayer]->endPhase();
    m_players[GetEnemyId(m_currentPlayer)]->endPhase();
}

void Battle::runBattleshockPhase() {
    auto unit = m_players[m_currentPlayer]->startPhase(Phase::Battleshock);
    while (unit) {
        unit->battleshock(m_currentPlayer);

        unit = m_players[m_currentPlayer]->advancePhase();
    }
    unit = m_players[GetEnemyId(m_currentPlayer)]->startPhase(Phase::Battleshock);
    while (unit) {
        unit->battleshock(m_currentPlayer);

        unit = m_players[GetEnemyId(m_currentPlayer)]->advancePhase();
    }

    m_players[m_currentPlayer]->endPhase();
    m_players[GetEnemyId(m_currentPlayer)]->endPhase();
}

void Battle::deployment() {

    // initiative to select player deploying first

    // loop until all units have been deployed

}
