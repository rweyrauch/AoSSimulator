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
    m_roster->doHeroPhase();
}

void Player::doMovementPhase()
{
    m_roster->doMovementPhase();
}

void Player::doShootingPhase()
{
    m_roster->doShootingPhase();
}

void Player::doChargePhase()
{
    m_roster->doChargePhase();
}

void Player::doCombatPhase()
{
    m_roster->doCombatPhase();
}

void Player::doBattleshockPhase()
{
    m_roster->doBattleshockPhase();
}

void Player::beginTurn()
{
    m_roster->beginTurn();
}
