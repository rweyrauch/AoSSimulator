/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cassert>
#include "Dice.h"

Dice::Dice() :
    m_rd(),
    m_gen(m_rd()),
    m_d6(1, 6)
{}

int Dice::rollD6()
{
    return m_d6(m_gen);
}

int Dice::roll2D6()
{
    return (m_d6(m_gen) + m_d6(m_gen));
}

int Dice::roll3D6()
{
    return (m_d6(m_gen) + m_d6(m_gen) + m_d6(m_gen));
}

int Dice::roll4D6()
{
    return (m_d6(m_gen) + m_d6(m_gen) + m_d6(m_gen) + m_d6(m_gen));
}

int Dice::rollD3()
{
    return (m_d6(m_gen)+1)/2;
}

std::vector<int> Dice::rollD6(int number)
{
    std::vector<int> result(number);
    for (auto i = 0; i < number; i++)
    {
        result[i] = m_d6(m_gen);
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
        result[i] = m_d6(m_gen);
        if (result[i] <= rerolling)
        {
            result[i] = m_d6(m_gen);
        }
    }
    return result;
}
