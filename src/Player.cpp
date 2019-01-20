/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Player.h>

void Player::doHeroPhase()
{
    if (m_roster)
        m_roster->doHeroPhase();
}

void Player::doMovementPhase()
{
    if (m_roster)
        m_roster->doMovementPhase();
}

void Player::doShootingPhase()
{
    if (m_roster)
        m_roster->doShootingPhase();
}

void Player::doChargePhase()
{
    if (m_roster)
        m_roster->doChargePhase();
}

void Player::doCombatPhase()
{
    if (m_roster)
        m_roster->doCombatPhase();
}

void Player::doBattleshockPhase()
{
    if (m_roster)
        m_roster->doBattleshockPhase();
}

void Player::beginTurn(int battleRound)
{
    if (m_roster)
        m_roster->beginTurn(battleRound);
}
