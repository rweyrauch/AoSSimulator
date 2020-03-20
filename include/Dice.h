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

    static int rollD6();

    static int roll2D6();

    static int roll3D6();

    static int roll4D6();

    static int rollD3();

    static int rollD4();

    static int rollSpecial(int number);

    static std::vector<int> rollD6(int number);

    static std::vector<int> rollD6(int number, int rerolling);

    struct RollResult {
        // distribution[0] is defined just make index values match roll values.
        // That is distribution[1] is the number of 1's rolled and distibution[6] is the number
        // of 6's rolled.
        int distribution[7] = {0, 0, 0, 0, 0, 0, 0};

        void clear() {
            for (auto i = 0; i < 7; i++)
                distribution[i] = 0;
        }

        void add(const RollResult &r) {
            for (auto i = 0; i < 7; i++)
                distribution[i] += r.distribution[i];
        }

        int rollsGE(int roll) const {
            if (roll < 1 || roll > 6) return 0;

            int rolls = 0;
            for (auto i = roll; i < 7; i++)
                rolls += distribution[i];
            return rolls;
        }

        void clearLT(int roll) {
            if (roll < 1 || roll > 6) return;
            for (auto i = 1; i < roll; i++) {
                distribution[i] = 0;
            }
        }

        int numUnmodified6s() const {
            return distribution[6];
        }

        int numUnmodified1s() const {
            return distribution[1];
        }
    };

    static void rollD6(int number, RollResult &result);

    static void rollD6(int number, int rerolling, RollResult &result);

private:

    // Only create/initialize the random number generator once.
    static std::random_device s_rd;
    static std::mt19937 s_gen;
    static std::uniform_int_distribution<int> s_d6;
    static std::uniform_int_distribution<int> s_d4;
};


#endif //DICE_H
