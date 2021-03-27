/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Player.h>

void Player::beginTurn(int battleRound, PlayerId playerWithTurn) {
    if (m_roster) {
        m_roster->beginTurn(battleRound, playerWithTurn);
    }
}

Unit *Player::startPhase(Phase phase) {
    // TODO: rosters/units to perform start of phase actions...
    m_currentPhase = phase;
    if (m_roster) {
        m_activatedUnit = m_roster->unitBegin();
        return m_activatedUnit->get();
    }
    return nullptr;
}

Unit *Player::advancePhase() {
    if (m_roster && (m_activatedUnit != m_roster->unitEnd())) {
        auto unit = *m_activatedUnit;
        ++m_activatedUnit;
        return unit.get();
    }
    return nullptr;
}

void Player::endPhase() {
    if (m_roster) {
        m_activatedUnit = m_roster->unitEnd();
    }
}

void Player::endTurn(int battleRound) {
    if (m_roster) {
        m_roster->endTurn(battleRound);
    }
}
