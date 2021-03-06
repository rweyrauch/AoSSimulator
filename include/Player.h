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

    void setRoster(std::shared_ptr<Roster> roster) {
        m_roster = roster;
        if (m_roster)
            m_activatedUnit = m_roster->end();
    }

    void beginTurn(int battleRound, PlayerId playerWithTurn);

    void endTurn(int battleRound);

    Unit *startPhase(GamePhase phase);

    Unit *advancePhase();

    void endPhase();

private:
    const PlayerId m_id;
    std::shared_ptr<Roster> m_roster = nullptr;

    GamePhase m_currentPhase = GamePhase::Initiative;
    std::list<std::shared_ptr<Unit>>::iterator m_activatedUnit;
};
