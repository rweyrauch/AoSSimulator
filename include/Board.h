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
#include <WarhammerSim.h>
#include <TerrainFeature.h>
#include "MathUtils.h"

class Roster;
class Unit;

struct Objective
{
    int m_id;
    float m_x, m_y;
};

class Board
{
public:
    static Board* Instance();

    void setSize(float width, float depth)
    {
        m_width = width;
        m_depth = depth;
    }

    void addFeature(TerrainFeature* feature);
    void addObjective(Objective* objective);
    void moveObjective(int which, float x, float y);
    void addRosters(Roster* pRedRoster, Roster* pBlueRoster);

    float width() const { return m_width; }
    float depth() const { return m_depth; }

    int getNumFeatures() const { return (int)m_features.size(); }
    const TerrainFeature* getFeature(int which) const { return m_features.at((size_t)which); }

    int getNumObjectives() const { return (int)m_objectives.size(); }
    const Objective* getObjective(int which) const { return m_objectives.at((size_t)which); }

    Roster* getPlayerRoster(PlayerId which) { return m_rosters[(int)which]; }

    Unit* getNearestUnit(const Unit* unit, PlayerId fromPlayer);
    std::vector<Unit*> getUnitsWithin(const Unit* unit, PlayerId fromPlayer, float distance);
    std::vector<Unit*> getUnitsWithin(const Math::Point3& point, PlayerId fromPlayer, float distance);
    Unit* getUnitWithKeyword(const Unit *unit, PlayerId fromPlayer, Keyword keyword, float distance);

    enum Quadrant
    {
        Northwest = 0, //
        Northeast,
        Southeast,
        Southwest,
    };

    std::vector<Unit*> getUnitWithin(Quadrant quadrant, PlayerId fromPlayer);
    std::vector<Unit*> getAllUnits(PlayerId fromPlayer);
    bool isUnitWithin(Quadrant quadrant, const Unit* unit) const;

    bool unbindAttempt(const Unit* caster, int castingRoll);

    void render(const std::string& filename) const;

protected:
    Board() = default;

    bool inQuadrant(Quadrant quadrant, const Math::Plane& northSouth, const Math::Plane& eastWest, const Math::Point3& point3) const;

private:
    float m_width = 0.0f, m_depth = 0.0f;
    std::vector<TerrainFeature*> m_features;
    std::vector<Objective*> m_objectives;
    Roster* m_rosters[2] = {nullptr, nullptr};

    static Board* s_pInstance;
};


#endif //WARHAMMERSIM_BOARD_H
