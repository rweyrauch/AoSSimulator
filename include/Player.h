/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Roster.h>

class Player {
public:
    explicit Player(PlayerId id) : m_id(id) {}

    void setRoster(Roster *roster) {
        m_roster = roster;
        if (m_roster)
            m_activatedUnit = m_roster->unitEnd();
    }

    void beginTurn(int battleRound, PlayerId playerWithTurn);

    Unit* startPhase(Phase phase);

    Unit* advancePhase();

    void endPhase();

private:
    const PlayerId m_id;
    Roster *m_roster = nullptr;

    Phase m_currentPhase = Phase::Initiative;
    std::list<Unit *>::iterator m_activatedUnit;
};
