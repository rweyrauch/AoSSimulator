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
    Roster(PlayerId id) : m_id(id) {}

    void addUnit(Unit* unit)
    {
        if (unit == nullptr) return;
        unit->setOwningPlayer(m_id);
        m_units.push_back(unit);
    }

    void beginTurn(int battleRound);
    void doHeroPhase();
    void doMovementPhase();
    void doShootingPhase();
    void doChargePhase();
    void doCombatPhase();
    void doBattleshockPhase();

    Unit* nearestUnit(const Unit* unit) const;

    std::vector<Unit*>::const_iterator unitBegin() const { return m_units.begin(); }
    std::vector<Unit*>::const_iterator unitEnd() const { return m_units.end(); }

    int totalPoints() const;

private:
    const PlayerId m_id;
    std::vector<Unit*> m_units;
};


#endif //ROSTER_H
