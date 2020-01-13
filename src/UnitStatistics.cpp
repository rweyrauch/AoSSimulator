/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitStatistics.h>

float UnitStatistics::totalMovementDistance() const
{
    float totalDistance = 0.0f;
    for (auto ip : m_actions)
    {
        totalDistance += ip.m_moved;
    }
    return totalDistance;
}

float UnitStatistics::averageMovementDistance() const
{
    int numRounds = numberOfRoundsMoved();
    if (numRounds <= 0)
    { return 0.0f; }
    return totalMovementDistance() / numRounds;
}

float UnitStatistics::totalRunDistance() const
{
    float totalDistance = 0.0f;
    for (auto ip : m_actions)
    {
        totalDistance += ip.m_ran;
    }
    return totalDistance;
}

float UnitStatistics::averageRunDistance() const
{
    int numRounds = numberOfRoundsRan();
    if (numRounds <= 0) { return 0.0f; }
    return totalRunDistance() / numRounds;
}

float UnitStatistics::totalChargeDistance() const
{
    float totalDistance = 0.0f;
    for (auto ip : m_actions)
    {
        totalDistance += ip.m_charged;
    }
    return totalDistance;
}

float UnitStatistics::averageChargeDistance() const
{
    int numRounds = numberOfRoundsCharged();
    if (numRounds <= 0) { return 0.0f; }
    return totalChargeDistance() / numRounds;
}

int UnitStatistics::totalEnemyModelsSlain() const
{
    int totalSlain = 0;
    for (auto ip : m_actions)
    {
        totalSlain += ip.m_enemyModelsSlain;
    }
    return totalSlain;
}

void UnitStatistics::totalWoundsInflicted(Wounds& wounds) const
{
    wounds = {0, 0};
    for (auto ip : m_actions)
    {
        wounds.normal += ip.m_woundsInflicted.normal;
        wounds.mortal += ip.m_woundsInflicted.mortal;
    }
}

int UnitStatistics::totalModelsSlain() const
{
    int totalSlain = 0;
    for (auto ip : m_actions)
    {
        totalSlain += ip.m_modelsSlain;
    }
    return totalSlain;
}

void UnitStatistics::totalWoundsTaken(Wounds& wounds) const
{
    wounds = {0, 0};
    for (auto ip : m_actions)
    {
        wounds.normal += ip.m_woundsTaken.normal;
        wounds.mortal += ip.m_woundsTaken.mortal;
    }
}

int UnitStatistics::numberOfRoundsMoved() const
{
    int numRounds = 0;
    for (auto ip : m_actions)
    {
        if (ip.m_moved > 0.0f)
        { numRounds++; }
    }
    return numRounds;
}

int UnitStatistics::numberOfRoundsRan() const
{
    int numRounds = 0;
    for (auto ip : m_actions)
    {
        if (ip.m_ran > 0.0f)
        { numRounds++; }
    }
    return numRounds;
}

int UnitStatistics::numberOfRoundsCharged() const
{
    int numRounds = 0;
    for (auto ip : m_actions)
    {
        if (ip.m_charged > 0.0f)
        { numRounds++; }
    }
    return numRounds;
}

int UnitStatistics::totalSavesMade() const
{
    int totalSaves = 0;
    for (auto ip : m_actions)
    {
        totalSaves += ip.m_savesMade;
    }
    return totalSaves;
}

int UnitStatistics::totalSavesFailed() const
{
    int totalSaves = 0;
    for (auto ip : m_actions)
    {
        totalSaves += ip.m_savesFailed;
    }
    return totalSaves;
}

void UnitStatistics::visitTurn(std::function<void(const TurnRecord &)> &visitor)
{
    for (auto ip : m_actions)
    {
        visitor(ip);
    }
}

int UnitStatistics::totalModelsFled() const
{
    int totalFled = 0;
    for (auto ip : m_actions)
    {
        totalFled += ip.m_numFled;
    }
    return totalFled;
}

void TurnRecord::clear()
{
    m_round = 0;
    m_playerWithTurn = PlayerId::None;

    m_moved = 0.0f;
    m_ran = 0.0f;
    m_charged = 0.0f;

    m_attacksMade = 0;
    m_attacksHitting = 0;

    m_savesMade = 0;
    m_savesFailed = 0;

    m_enemyModelsSlain = 0;
    m_woundsInflicted = {0, 0};

    m_modelsSlain = 0;
    m_woundsTaken = {0, 0};

    m_numFled = 0;
}
