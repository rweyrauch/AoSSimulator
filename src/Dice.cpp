/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cassert>
#include <Dice.h>

std::random_device Dice::s_rd;
std::mt19937 Dice::s_gen(s_rd());
std::uniform_int_distribution<int> Dice::s_d6(1, 6);

Dice::Dice()
{}

int Dice::rollD6()
{
    return s_d6(s_gen);
}

int Dice::roll2D6()
{
    return (s_d6(s_gen) + s_d6(s_gen));
}

int Dice::roll3D6()
{
    return (s_d6(s_gen) + s_d6(s_gen) + s_d6(s_gen));
}

int Dice::roll4D6()
{
    return (s_d6(s_gen) + s_d6(s_gen) + s_d6(s_gen) + s_d6(s_gen));
}

int Dice::rollD3()
{
    return (s_d6(s_gen)+1)/2;
}

std::vector<int> Dice::rollD6(int number)
{
    std::vector<int> result(number);
    for (auto i = 0; i < number; i++)
    {
        result[i] = s_d6(s_gen);
    }
    return result;
}

std::vector<int> Dice::rollD6(int number, int rerolling)
{
    assert(rerolling >= 1);
    assert(rerolling <= 6);

    std::vector<int> result(number);
    for (auto i = 0; i < number; i++)
    {
        result[i] = s_d6(s_gen);
        if (result[i] <= rerolling)
        {
            result[i] = s_d6(s_gen);
        }
    }
    return result;
}

void Dice::rollD6(int number, Dice::RollResult &result)
{
    result.clear();
    for (auto i = 0; i < number; i++)
    {
        result.distribution[s_d6(s_gen)]++;
    }
}

void Dice::rollD6(int number, int rerolling, Dice::RollResult &result)
{
    assert(rerolling >= 1);
    assert(rerolling <= 6);

    result.clear();

    for (auto i = 0; i < number; i++)
    {
        int roll = s_d6(s_gen);
        if (roll <= rerolling)
        {
            roll = s_d6(s_gen);
        }
        result.distribution[roll]++;
    }
}
