/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include "Dice.h"

const int g_numRolls = 10000;

TEST(Dice, D6)
{
    Dice dice;

    for (auto i = 0; i < g_numRolls; i++)
    {
        auto r = dice.rollD6();
        ASSERT_GE(r, 1);
        ASSERT_LE(r, 6);
    }
}

TEST(Dice, D3)
{
    Dice dice;

    for (auto i = 0; i < g_numRolls; i++)
    {
        auto r = dice.rollD3();
        ASSERT_GE(r, 1);
        ASSERT_LE(r, 3);
    }
}

TEST(Dice, TwoD6)
{
    Dice dice;

    for (auto i = 0; i < g_numRolls; i++)
    {
        auto r = dice.roll2D6();
        ASSERT_GE(r, 2);
        ASSERT_LE(r, 12);
    }
}

TEST(Dice, ThreeD6)
{
    Dice dice;

    for (auto i = 0; i < g_numRolls; i++)
    {
        auto r = dice.roll3D6();
        ASSERT_GE(r, 3);
        ASSERT_LE(r, 18);
    }
}

TEST(Dice, MultipleD6)
{
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist(2, 100);

    Dice dice;

    for (auto i = 0; i < g_numRolls; i++)
    {
        auto numDice = (int)dist(gen);

        auto rv = dice.rollD6(numDice);
        ASSERT_EQ(rv.size(), numDice);
        for (auto r : rv)
        {
            ASSERT_GE(r, 1);
            ASSERT_LE(r, 6);
        }
    }
}

TEST(Dice, MultipleD6Rerolling)
{
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist(2, 100);

    Dice dice;

    for (auto i = 0; i < g_numRolls; i++)
    {
        auto numDice = (int)dist(gen);

        auto rv = dice.rollD6(numDice, 1);
        ASSERT_EQ(rv.size(), numDice);
        for (auto r : rv)
        {
            ASSERT_GE(r, 1);
            ASSERT_LE(r, 6);
        }
    }
}
