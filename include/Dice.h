/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_DICE_H
#define WARHAMMERSIM_DICE_H

#include <random>
#include <vector>

class Dice
{
public:
    Dice();

    int rollD6();
    int roll2D6();
    int roll3D6();
    int roll4D6();
    int rollD3();

    std::vector<int> rollD6(int number);
    std::vector<int> rollD6(int number, int rerolling);

private:
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_int_distribution<int> m_d6;
};


#endif //WARHAMMERSIM_DICE_H
