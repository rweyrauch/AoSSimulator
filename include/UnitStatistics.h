/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNITSTATISTICS_H
#define UNITSTATISTICS_H

#include <vector>
#include <functional>
#include "WarhammerSim.h"

struct TurnRecord
{
    void clear();

    int m_round = 0;
    PlayerId m_playerWithTurn = PlayerId::None;

    float m_moved = 0.0f;
    float m_ran = 0.0f;
    float m_charged = 0.0f;

    int m_attacksMade = 0;
    int m_attacksHitting = 0;

    int m_savesMade = 0;
    int m_savesFailed = 0;

    int m_enemyModelsSlain = 0;
    Wounds m_woundsInflicted = {0, 0};

    int m_modelsSlain = 0;
    Wounds m_woundsTaken = {0, 0};

    int m_numFled = 0;
};

class UnitStatistics
{
public:
    UnitStatistics() = default;

    void record(const TurnRecord& action)
    {
        m_actions.push_back(action);
    }

    int numberOfRoundsMoved() const;
    int numberOfRoundsRan() const;
    int numberOfRoundsCharged() const;

    // Totals
    float totalMovementDistance() const;
    float averageMovementDistance() const;
    float totalRunDistance() const;
    float averageRunDistance() const;
    float totalChargeDistance() const;
    float averageChargeDistance() const;

    int totalSavesMade() const;
    int totalSavesFailed() const;

    int totalEnemyModelsSlain() const;
    Wounds totalWoundsInflicted() const;

    int totalModelsSlain() const;
    Wounds totalWoundsTaken() const;

    int totalModelsFled() const;

    void reset()
    {
        m_numRounds = 0;
        m_actions.clear();
    }

    void visitTurn(std::function<void(const TurnRecord&)>& visitor);

private:

    int m_numRounds = 0;
    std::vector<TurnRecord> m_actions;
};


#endif //UNITSTATISTICS_H
