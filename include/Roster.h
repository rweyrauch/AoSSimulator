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
    explicit Roster(PlayerId id) : m_id(id) {}
    ~Roster();

    PlayerId getOwningPlayer() const { return m_id; }

    void addUnit(Unit* unit)
    {
        if (unit == nullptr) return;
        unit->setRoster(this);
        m_units.push_back(unit);
    }

    void beginRound(int battleRound);
    void endRound(int battleRound);
    void beginTurn(int battleRound, PlayerId playerWithTurn);
    void endTurn(int battleRound);
    void doHeroPhase();
    void doMovementPhase();
    void doShootingPhase();
    void doChargePhase();
    void doCombatPhase();
    void doBattleshockPhase();

    Unit* nearestUnit(const Unit* unit) const;

    std::vector<Unit*>::iterator unitBegin() { return m_units.begin(); }
    std::vector<Unit*>::iterator unitEnd() { return m_units.end(); }

    int totalPoints() const;

    void setCommandPoints(int cp) { m_cp = cp; }
    void addCommandPoints(int cp) { m_cp += cp; }
    int getCommandPoints() const { return m_cp; }
    bool useCommandPoint();

private:
    const PlayerId m_id;
    std::vector<Unit*> m_units;

    int m_cp = 0;
};


#endif //ROSTER_H
