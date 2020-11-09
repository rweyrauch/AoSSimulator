/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROSTER_H
#define ROSTER_H

#include <list>
#include <Unit.h>

class Roster {
public:
    explicit Roster(PlayerId id) : m_id(id) {}

    ~Roster();

    [[nodiscard]] PlayerId getOwningPlayer() const { return m_id; }

    void addUnit(Unit *unit);

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

    Unit *nearestUnit(const Unit *unit) const;

    std::list<Unit *>::iterator unitBegin() { return m_units.begin(); }

    std::list<Unit *>::iterator unitEnd() { return m_units.end(); }

    [[nodiscard]] int totalPoints() const;

    void setCommandPoints(int cp) { m_cp = cp; }

    void addCommandPoints(int cp) { m_cp += cp; }

    [[nodiscard]] int getCommandPoints() const { return m_cp; }

    bool useCommandPoint();

private:
    const PlayerId m_id;
    std::list<Unit *> m_units; // NOTE: using a list so that units may be added while iterating over the list

    int m_cp = 0;
};


#endif //ROSTER_H
