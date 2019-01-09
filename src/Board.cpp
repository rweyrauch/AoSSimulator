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
#include <Roster.h>

Board* Board::s_pInstance = nullptr;

void Board::addObjective(Objective* objective)
{
    m_objectives.push_back(objective);
}

void Board::moveObjective(int id, float x, float y)
{
    auto matchId = [id](const Objective* obj)->bool { return (obj->m_id == id); };
    auto obj = std::find_if(m_objectives.begin(), m_objectives.end(), matchId);
    if (obj != m_objectives.end())
    {
        (*obj)->m_x = x;
        (*obj)->m_y = y;
    }
}

void Board::addFeature(TerrainFeature* feature)
{
    m_features.push_back(feature);
}

void Board::addRosters(const Roster *pRedRoster, const Roster *pBlueRoster)
{
    m_rosters[(int)PlayerId::Red] = pRedRoster;
    m_rosters[(int)PlayerId::Blue] = pBlueRoster;
}

Board *Board::Instance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new Board();
    }
    return s_pInstance;
}
