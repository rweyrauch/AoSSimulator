/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <memory>
#include <list>
#include <Unit.h>

// Simple counting resources for Factions
enum class Resource : int {
    None = 0,
    Khorne_Blood_Tithe,
    Slaanesh_Depravity,
    BeastsOfChaos_Primordial_Call,
    Nurgle_Contagion,
    Skaven_Warpstone_Spark,
    Seraphon_Celestial_Conjuration,
    Tzeentch_Fate_Points,
};

class Roster {
public:
    explicit Roster(PlayerId id) : m_id(id) {}

    ~Roster();

    [[nodiscard]] PlayerId getOwningPlayer() const { return m_id; }

    void addUnit(std::shared_ptr<Unit> unit);

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

    void restore();

    int getPoints() const;

    Unit *nearestUnit(const Unit *unit) const;

    std::list<std::shared_ptr<Unit>>::iterator unitBegin() { return m_units.begin(); }

    std::list<std::shared_ptr<Unit>>::iterator unitEnd() { return m_units.end(); }

    [[nodiscard]] int totalPoints() const;

    void setCommandPoints(int cp) { m_cp = cp; }

    void addCommandPoints(int cp) { m_cp += cp; }

    [[nodiscard]] int getCommandPoints() const { return m_cp; }

    bool useCommandPoint();

    Resource getResourceType() const { return m_factionResource; }
    void incrementResource(int value) { m_resourceCount += value; }

    int getAvailableResource() const { return m_resourceCount; }
    void clearAvailableResource() { m_resourceCount = 0; }

    Unit* getGeneral() const;

private:
    const PlayerId m_id;
    std::list<std::shared_ptr<Unit>> m_units; // NOTE: using a list so that units may be added while iterating over the list

    int m_cp = 0;

    // Faction-specific earned resources
    Resource m_factionResource = Resource::None;
    int m_resourceCount = 0;
};

