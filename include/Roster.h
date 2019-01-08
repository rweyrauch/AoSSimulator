/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROSTER_H
#define ROSTER_H

#include <vector>
#include <Unit.h>

class Roster
{
public:
    Roster() = default;

    void addUnit(const Unit& unit)
    {
        m_units.push_back(unit);
    }

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


#endif //ROSTER_H
