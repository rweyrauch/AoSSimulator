/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cassert>
#include <Dice.h>
#include <AgeOfSigmarSim.h>

std::random_device Dice::s_rd;
std::mt19937 Dice::s_gen(s_rd());
std::uniform_int_distribution<int> Dice::s_d6(1, 6);
std::uniform_int_distribution<int> Dice::s_d4(1, 4);

Dice::Dice() = default;

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
    return (s_d6(s_gen) + 1) / 2;
}

int Dice::rollD4()
{
    return (s_d4(s_gen));
}

std::vector<int> Dice::rollD6(int number)
{
    std::vector<int> result((size_t) number);
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

    std::vector<int> result((size_t) number);
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

int Dice::rollSpecial(int number)
{
    if (number >= 0)
    {
        return number;
    }
    else if (number == RAND_D3)
    {
        return rollD3();
    }
    else if (number == RAND_2D3)
    {
        return rollD3() + rollD3();
    }
    else if (number == RAND_D6)
    {
        return rollD6();
    }
    else if (number == RAND_2D6)
    {
        return roll2D6();
    }
    else if (number == RAND_3D6)
    {
        return roll3D6();
    }
    else if (number == RAND_4D6)
    {
        return roll4D6();
    }
    return 0;
}
