/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <vector>
#include <functional>
#include "AgeOfSigmarSim.h"

struct TurnRecord {
    void clear();

    int m_round = 0;
    PlayerId m_playerWithTurn = PlayerId::None;

    double m_moved = 0;
    double m_ran = 0;
    double m_charged = 0;

    int m_attacksMade = 0;
    int m_attacksHitting = 0;

    int m_savesMade = 0;
    int m_savesFailed = 0;

    int m_enemyModelsSlain = 0;
    Wounds m_woundsInflicted = {0, 0};

    int m_enemyUnitsSlain = 0;

    int m_modelsSlain = 0;
    Wounds m_woundsTaken = {0, 0};

    int m_herosSlain = 0;
    int m_monstersSlain = 0;

    int m_numFled = 0;
};

class UnitStatistics {
public:
    UnitStatistics() = default;

    void record(const TurnRecord &action) {
        m_actions.push_back(action);
    }

    [[nodiscard]] int numberOfRoundsMoved() const;

    [[nodiscard]] int numberOfRoundsRan() const;

    [[nodiscard]] int numberOfRoundsCharged() const;

    // Totals
    [[nodiscard]] double totalMovementDistance() const;

    [[nodiscard]] double averageMovementDistance() const;

    [[nodiscard]] double totalRunDistance() const;

    [[nodiscard]] double averageRunDistance() const;

    [[nodiscard]] double totalChargeDistance() const;

    [[nodiscard]] double averageChargeDistance() const;

    [[nodiscard]] int totalSavesMade() const;

    [[nodiscard]] int totalSavesFailed() const;

    [[nodiscard]] int totalEnemyModelsSlain() const;

    void totalWoundsInflicted(Wounds &wounds) const;

    [[nodiscard]] int totalModelsSlain() const;

    void totalWoundsTaken(Wounds &wounds) const;

    [[nodiscard]] int totalModelsFled() const;

    void reset() {
        m_numRounds = 0;
        m_actions.clear();
    }

    void visitTurn(std::function<void(const TurnRecord &)> &visitor);

private:

    int m_numRounds = 0;
    std::vector<TurnRecord> m_actions;
};

