/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_ARMY_H
#define WARHAMMERSIM_ARMY_H

#include <vector>
#include <Unit.h>

class Roster
{
public:
    Roster() = default;

    void beginTurn();
    void doHeroPhase();
    void doMovementPhase();
    void doShootingPhase();
    void doChargePhase();
    void doCombatPhase();
    void doBattleshockPhase();

private:
    std::vector<Unit> m_units;
};


#endif //WARHAMMERSIM_ARMY_H
