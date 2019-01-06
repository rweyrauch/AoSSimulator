/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <vector>
#include <algorithm>
#include <Board.h>

void Board::addObjective(const Objective &objective)
{
    m_objectives.push_back(objective);
}

void Board::moveObjective(int id, float x, float y)
{
    auto matchId = [id](const Objective& obj)->bool { return (obj.m_id == id); };
    auto obj = std::find_if(m_objectives.begin(), m_objectives.end(), matchId);
    if (obj != m_objectives.end())
    {
        obj->m_x = x;
        obj->m_y = y;
    }
}

void Board::addFeature(const TerrainFeature &feature)
{
    m_features.push_back(feature);
}
