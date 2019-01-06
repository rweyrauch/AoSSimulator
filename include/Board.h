/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_BOARD_H
#define WARHAMMERSIM_BOARD_H

#include <vector>
#include <TerrainFeature.h>

struct Objective
{
    int m_id;
    float m_x, m_y;
};

class Board
{
public:
    Board() = default;

    void setSize(float width, float depth)
    {
        m_width = width;
        m_depth = depth;
    }

    void addFeature(const TerrainFeature& feature);
    void addObjective(const Objective& objective);
    void moveObjective(int id, float x, float y);

private:
    float m_width = 0.0f, m_depth = 0.0f;
    std::vector<TerrainFeature> m_features;
    std::vector<Objective> m_objectives;
};


#endif //WARHAMMERSIM_BOARD_H
