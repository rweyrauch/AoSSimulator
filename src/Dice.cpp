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

int Dice::RollD6() {
    return s_d6(s_gen);
}

int Dice::Roll2D6() {
    return (s_d6(s_gen) + s_d6(s_gen));
}

int Dice::Roll3D6() {
    return (s_d6(s_gen) + s_d6(s_gen) + s_d6(s_gen));
}

int Dice::Roll4D6() {
    return (s_d6(s_gen) + s_d6(s_gen) + s_d6(s_gen) + s_d6(s_gen));
}

int Dice::RollD3() {
    return (s_d6(s_gen) + 1) / 2;
}

int Dice::RollD4() {
    return (s_d4(s_gen));
}

std::vector<int> Dice::RollD6(int number) {
    std::vector<int> result((size_t) number);
    for (auto i = 0; i < number; i++) {
        result[i] = s_d6(s_gen);
    }
    return result;
}

std::vector<int> Dice::RollD6(int number, int rerolling) {
    assert(rerolling >= 1);
    assert(rerolling <= 6);

    std::vector<int> result((size_t) number);
    for (auto i = 0; i < number; i++) {
        result[i] = s_d6(s_gen);
        if (result[i] <= rerolling) {
            result[i] = s_d6(s_gen);
        }
    }
    return result;
}

void Dice::RollD6(int number, Dice::RollResult &result) {
    result.clear();
    for (auto i = 0; i < number; i++) {
        result.m_distribution[s_d6(s_gen)]++;
    }
}

void Dice::RollD6(int number, int rerolling, Dice::RollResult &result) {
    assert(rerolling >= 1);
    assert(rerolling <= 6);

    result.clear();

    for (auto i = 0; i < number; i++) {
        int roll = s_d6(s_gen);
        if (roll <= rerolling) {
            roll = s_d6(s_gen);
        }
        result.m_distribution[roll]++;
    }
}

int Dice::RollSpecial(int number) {
    if (number >= 0) {
        return number;
    } else if (number == RAND_D3) {
        return RollD3();
    } else if (number == RAND_2D3) {
        return RollD3() + RollD3();
    } else if (number == RAND_D6) {
        return RollD6();
    } else if (number == RAND_2D6) {
        return Roll2D6();
    } else if (number == RAND_3D6) {
        return Roll3D6();
    } else if (number == RAND_4D6) {
        return Roll4D6();
    }
    return 0;
}
