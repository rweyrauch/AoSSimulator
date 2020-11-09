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

class Dice {
public:
    Dice() = delete;

    ~Dice() = delete;

    static int RollD6();

    static int Roll2D6();

    static int Roll3D6();

    static int Roll4D6();

    static int RollD3();

    static int RollD4();

    static int RollSpecial(int number);

    static std::vector<int> RollD6(int number);

    static std::vector<int> RollD6(int number, int rerolling);

    struct RollResult {
        // distribution[0] is defined just make index values match roll values.
        // That is distribution[1] is the number of 1's rolled and distibution[6] is the number
        // of 6's rolled.
        int m_distribution[7] = {0, 0, 0, 0, 0, 0, 0};

        void clear() {
            for (auto i = 0; i < 7; i++)
                m_distribution[i] = 0;
        }

        void add(const RollResult &r) {
            for (auto i = 0; i < 7; i++)
                m_distribution[i] += r.m_distribution[i];
        }

        [[nodiscard]] int rollsGE(int roll) const {
            if (roll < 1 || roll > 6) return 0;

            int rolls = 0;
            for (auto i = roll; i < 7; i++)
                rolls += m_distribution[i];
            return rolls;
        }

        void clearLT(int roll) {
            if (roll < 1 || roll > 6) return;
            for (auto i = 1; i < roll; i++) {
                m_distribution[i] = 0;
            }
        }

        [[nodiscard]] int numUnmodified6s() const {
            return m_distribution[6];
        }

        [[nodiscard]] int numUnmodified1s() const {
            return m_distribution[1];
        }
    };

    static void RollD6(int number, RollResult &result);

    static void RollD6(int number, int rerolling, RollResult &result);

private:

    // Only create/initialize the random number generator once.
    static std::random_device s_rd;
    static std::mt19937 s_gen;
    static std::uniform_int_distribution<int> s_d6;
    static std::uniform_int_distribution<int> s_d4;
};


#endif //DICE_H
