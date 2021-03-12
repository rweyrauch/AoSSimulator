/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cfloat>
#include <plog/Log.h>
#include <Roster.h>
#include "AoSSimPrivate.h"

constexpr std::array<int, 8> g_factionResourceName = {
        ToInteger(Resource::None),
        ToInteger(Resource::Khorne_Blood_Tithe),
        ToInteger(Resource::Slaanesh_Depravity),
        ToInteger(Resource::BeastsOfChaos_Primordial_Call),
        ToInteger(Resource::Nurgle_Contagion),
        ToInteger(Resource::Skaven_Warpstone_Spark),
        ToInteger(Resource::Seraphon_Celestial_Conjuration),
        ToInteger(Resource::Tzeentch_Fate_Points),
};

void Roster::addUnit(std::shared_ptr<Unit> unit) {
    if (unit == nullptr) return;
    unit->setRoster(this);
    m_units.push_back(unit);

    // Automatically set resource type based on unit's faction
    if (unit->hasKeyword(KHORNE)) m_factionResource = Resource::Khorne_Blood_Tithe;
    else if (unit->hasKeyword(SLAANESH)) m_factionResource = Resource::Slaanesh_Depravity;
    else if (unit->hasKeyword(BEASTS_OF_CHAOS)) m_factionResource = Resource::BeastsOfChaos_Primordial_Call;
    else if (unit->hasKeyword(NURGLE)) m_factionResource = Resource::Nurgle_Contagion;
    else if (unit->hasKeyword(SKAVEN)) m_factionResource = Resource::Skaven_Warpstone_Spark;
    else if (unit->hasKeyword(SERAPHON)) m_factionResource = Resource::Seraphon_Celestial_Conjuration;
    else if (unit->hasKeyword(TZEENTCH)) m_factionResource = Resource::Tzeentch_Fate_Points;
    else m_factionResource = Resource::None;
}

void Roster::doHeroPhase() {
    for (auto u : m_units) {
        u->hero(m_id);
    }
}

void Roster::doMovementPhase() {
    for (auto u : m_units) {
        u->movement(m_id);
    }
}

void Roster::doShootingPhase() {
    for (auto u : m_units) {
        u->shooting(m_id);
    }
}

void Roster::doChargePhase() {
    for (auto u : m_units) {
        u->charge(m_id);
    }
}

void Roster::doCombatPhase() {
    for (auto u : m_units) {
        u->combat(m_id);
    }
}

void Roster::doBattleshockPhase() {
    for (auto u : m_units) {
        u->battleshock(m_id);
    }
}

void Roster::beginTurn(int battleRound, PlayerId playerWithTurn) {
    for (auto u : m_units) {
        u->beginTurn(battleRound, playerWithTurn);
    }
}

Unit *Roster::nearestUnit(const Unit *unit) const {
    auto nearestUnit = m_units.front();
    auto minDistance = DBL_MAX;
    for (auto u : m_units) {
        const auto dist = unit->distanceTo(u.get());
        if (dist < minDistance) {
            minDistance = dist;
            nearestUnit = u;
        }
    }
    return nearestUnit.get();
}

int Roster::totalPoints() const {
    int points = 0;
    for (auto u : m_units) {
        points += u->points();
    }
    return points;
}

void Roster::endTurn(int battleRound) {
    for (auto u : m_units) {
        u->endTurn(battleRound);
    }

    if (m_factionResource != Resource::None) {
        PLOG_INFO << "Player " << PlayerIdToString(m_id) << ":  Resource Name: "
                  << magic_enum::enum_name(m_factionResource) << "  Total: " << m_resourceCount;
    }
}

void Roster::beginRound(int battleRound) {
    for (auto u : m_units) {
        u->beginRound(battleRound);
    }
}

void Roster::endRound(int battleRound) {
    for (auto u : m_units) {
        u->endRound(battleRound);
    }
}

bool Roster::useCommandPoint() {
    int cp = getCommandPoints();
    if (cp > 0) {
        setCommandPoints(cp - 1);
        return true;
    }
    return false;
}

Roster::~Roster() = default;

void Roster::restore() {
    for (auto u : m_units) {
        u->restore();
    }
}

int Roster::getPoints() const {
    int totalPoints = 0;
    for (auto u : m_units) {
        totalPoints += u->points();
    }
    return totalPoints;
}

Unit *Roster::getGeneral() const {
    for (auto u : m_units) {
        if (u->isGeneral()) return u.get();
    }
    return nullptr;
}
