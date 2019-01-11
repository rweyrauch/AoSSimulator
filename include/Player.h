/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <Roster.h>

class Player
{
public:
    Player(PlayerId id) : m_id(id) {}

    void setRoster(Roster* roster)
    {
        m_roster = roster;
    }

    void beginTurn();
    void doHeroPhase();
    void doMovementPhase();
    void doShootingPhase();
    void doChargePhase();
    void doCombatPhase();
    void doBattleshockPhase();

private:
    const PlayerId m_id;
    Roster* m_roster = nullptr;
};


#endif //PLAYER_H
