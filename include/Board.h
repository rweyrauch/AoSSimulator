/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <AgeOfSigmarSim.h>
#include <TerrainFeature.h>
#include "MathUtils.h"

class Model;

class Roster;

class Unit;

struct Objective {
    int m_id;
    Math::Point3 m_pos;
};

class Board {
public:
    static Board *Instance();

    void setSize(double width, double depth) {
        m_width = width;
        m_depth = depth;
    }

    void addFeature(TerrainFeature *feature);

    void addObjective(Objective *objective);

    void moveObjective(int which, double x, double y);

    void addRosters(Roster *pRedRoster, Roster *pBlueRoster);

    double width() const { return m_width; }

    double depth() const { return m_depth; }

    int getNumFeatures() const { return (int) m_features.size(); }

    const TerrainFeature *getFeature(int which) const { return m_features.at((size_t) which); }

    int getNumObjectives() const { return (int) m_objectives.size(); }

    const Objective *getObjective(int which) const { return m_objectives.at((size_t) which); }

    const Objective *getNearestObjective(const Unit *unit);

    Roster *getPlayerRoster(PlayerId which) { return m_rosters[(int) which]; }

    Unit *getNearestUnit(const Unit *unit, PlayerId fromPlayer);

    std::vector<Unit *> getUnitsWithin(const Unit *unit, PlayerId fromPlayer, double distance);

    std::vector<Unit *> getUnitsWithin(const Math::Point3 &point, PlayerId fromPlayer, double distance);

    Unit *getUnitWithKeyword(const Unit *unit, PlayerId fromPlayer, Keyword keyword, double distance);

    void setRealm(Realm realm) {
        m_realm = realm;
    }

    Realm getRealm() const { return m_realm; }

    enum Quadrant {
        Northwest = 0, //
        Northeast,
        Southeast,
        Southwest,
    };

    std::vector<Unit *> getUnitWithin(Quadrant quadrant, PlayerId fromPlayer);

    std::vector<Unit *> getAllUnits(PlayerId fromPlayer);

    bool isUnitWithin(Quadrant quadrant, const Unit *unit) const;

    bool unbindAttempt(Unit *caster, int castingRoll);

    bool isVisible(const Unit *from, const Unit *target);

    void render(const std::string &filename) const;

    bool castRay(const Math::Ray &ray, Math::RayHit &result) const;

    bool castRay(const Math::Ray2 &ray, Math::RayHit &result) const;

    double moveModel(Model &model, const Math::Point3 &toPoint) const;

protected:
    Board() = default;

    bool inQuadrant(Quadrant quadrant, const Math::Plane &northSouth, const Math::Plane &eastWest,
                    const Math::Point3 &point3) const;

private:
    double m_width = 0.0f, m_depth = 0.0f;
    std::vector<TerrainFeature *> m_features;
    std::vector<Objective *> m_objectives;
    Roster *m_rosters[2] = {nullptr, nullptr};
    Realm m_realm = Azyr;

    static Board *s_pInstance;
};


#endif //WARHAMMERSIM_BOARD_H
