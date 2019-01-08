/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DICE_H
#define DICE_H

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

    struct RollResult
    {
        // distribution[0] is defined just make index values match roll values.
        // That is distribution[1] is the number of 1's rolled and distibution[6] is the number
        // of 6's rolled.
        int distribution[7] = { 0, 0, 0, 0, 0, 0, 0 };
        void clear()
        {
            for (auto i = 0; i < 7; i++)
                distribution[i] = 0;
        }

        int rollsGE(int roll) const
        {
            if (roll < 1 || roll > 6) return 0;

            int rolls = 0;
            for (auto i = roll; i < 7; i++)
                rolls += distribution[i];
            return rolls;
        }
        int numUnmodified6s() const
        {
            return distribution[7];
        }
    };

    void rollD6(int number, RollResult& result);
    void rollD6(int number, int rerolling, RollResult& result);

private:
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_int_distribution<int> m_d6;
};


#endif //DICE_H
