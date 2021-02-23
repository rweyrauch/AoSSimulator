/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <vector>
#include <memory>
#include <AgeOfSigmarSim.h>
#include <AoSKeywords.h>
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

    void addFeature(std::shared_ptr<TerrainFeature> feature);

    void addObjective(std::shared_ptr<Objective> objective);

    void moveObjective(int which, double x, double y);

    void addRosters(std::shared_ptr<Roster> pRedRoster, std::shared_ptr<Roster> pBlueRoster);

    [[nodiscard]] double width() const { return m_width; }

    [[nodiscard]] double depth() const { return m_depth; }

    [[nodiscard]] int getNumFeatures() const { return (int) m_features.size(); }

    [[nodiscard]] const TerrainFeature *getFeature(int which) const { return m_features.at((size_t) which).get(); }

    [[nodiscard]] int getNumObjectives() const { return (int) m_objectives.size(); }

    [[nodiscard]] const Objective *getObjective(int which) const { return m_objectives.at((size_t) which).get(); }

    const Objective *getNearestObjective(const Unit *unit);

    Roster *getPlayerRoster(PlayerId which) { return m_rosters[(int) which].get(); }

    Unit *getNearestUnit(const Unit *unit, PlayerId fromPlayer);

    std::vector<Unit *> getUnitsWithin(const Unit *unit, PlayerId fromPlayer, double distance);

    std::vector<Unit *> getUnitsWithin(const Math::Point3 &point, PlayerId fromPlayer, double distance);

    Unit *getUnitWithKeyword(const Unit *unit, PlayerId fromPlayer, Keyword keyword, double distance);

    std::vector<Unit *> getUnitsWithKeyword(PlayerId fromPlayer, Keyword keyword);

    void setRealm(Realm realm) {
        m_realm = realm;
    }

    [[nodiscard]] Realm getRealm() const { return m_realm; }

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

    [[nodiscard]] bool inQuadrant(Quadrant quadrant, const Math::Plane &northSouth, const Math::Plane &eastWest,
                    const Math::Point3 &point3) const;

private:
    double m_width = 0.0f, m_depth = 0.0f;
    std::vector<std::shared_ptr<TerrainFeature>> m_features;
    std::vector<std::shared_ptr<Objective>> m_objectives;
    std::shared_ptr<Roster> m_rosters[2] = {nullptr, nullptr};
    Realm m_realm = Azyr;

    static Board *s_pInstance;
};

