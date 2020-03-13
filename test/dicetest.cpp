/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include "Dice.h"
#include <lsignal.h>

const int g_numRolls = 10000;

TEST(Dice, D6)
{
    for (auto i = 0; i < g_numRolls; i++)
    {
        auto r = Dice::rollD6();
        ASSERT_GE(r, 1);
        ASSERT_LE(r, 6);
    }
}

TEST(Dice, D3)
{
    for (auto i = 0; i < g_numRolls; i++)
    {
        auto r = Dice::rollD3();
        ASSERT_GE(r, 1);
        ASSERT_LE(r, 3);
    }
}

TEST(Dice, TwoD6)
{
    for (auto i = 0; i < g_numRolls; i++)
    {
        auto r = Dice::roll2D6();
        ASSERT_GE(r, 2);
        ASSERT_LE(r, 12);
    }
}

TEST(Dice, ThreeD6)
{
    for (auto i = 0; i < g_numRolls; i++)
    {
        auto r = Dice::roll3D6();
        ASSERT_GE(r, 3);
        ASSERT_LE(r, 18);
    }
}

TEST(Dice, MultipleD6)
{
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist(2, 100);

    for (auto i = 0; i < g_numRolls; i++)
    {
        auto numDice = (int)dist(gen);

        auto rv = Dice::rollD6(numDice);
        ASSERT_EQ(rv.size(), numDice);
        for (auto r : rv)
        {
            ASSERT_GE(r, 1);
            ASSERT_LE(r, 6);
        }

        Dice::RollResult rr;
        Dice::rollD6(numDice, rr);
        int total = 0;
        for (int v : rr.distribution)
        {
            total += v;
        }
        ASSERT_EQ(total, numDice);
    }
}

TEST(Dice, MultipleD6Rerolling)
{
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist(2, 100);

    for (auto i = 0; i < g_numRolls; i++)
    {
        auto numDice = (int)dist(gen);

        auto rv = Dice::rollD6(numDice, 1);
        ASSERT_EQ(rv.size(), numDice);
        for (auto r : rv)
        {
            ASSERT_GE(r, 1);
            ASSERT_LE(r, 6);
        }

        Dice::RollResult rr;
        Dice::rollD6(numDice, 1, rr);
        int total = 0;
        for (int v : rr.distribution)
        {
            total += v;
        }
        ASSERT_EQ(total, numDice);
    }
}

int foo(int x) {
    return x+1;
}
int bar(int x) {
    return x+2;
}
int sum_acc(const std::vector<int>& v) {
    return std::accumulate(v.cbegin(), v.cend(), 0);
}

TEST(Dice, SignalTest)
{
    lsignal::signal<int(int)> worker;
    auto bc = worker.connect(bar);
    auto fc = worker.connect(foo);

    auto last = worker(2);
    auto total = worker(2, sum_acc);

    worker.disconnect(bc);
    worker.disconnect(fc);
}
