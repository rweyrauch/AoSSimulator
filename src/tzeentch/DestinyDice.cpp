/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "DestinyDice.h"

namespace Tzeentch {

    DestinyDice::DestinyDice() {
        initialize();
    }

    bool DestinyDice::addOne(int roll) {
        if (total() + 1 <= MAX_DESTINY_DICE) {
            m_dice.m_distribution[Dice::RollD6()]++;
            return true;
        }
        return false;
    }

    bool DestinyDice::add(int numRolls) {
        if (total() + numRolls <= MAX_DESTINY_DICE) {
            for (auto i = 0; i < numRolls; i++) {
                m_dice.m_distribution[Dice::RollD6()]++;
            }
            return true;
        }
        return false;
    }

    int DestinyDice::use(int roll) {
        return 0;
    }

    void DestinyDice::clear() {
        m_dice.clear();
    }

    void DestinyDice::initialize() {
        Dice::RollD6(MAX_DESTINY_DICE, m_dice);
    }

    bool DestinyDice::replace(int roll, int with) {
        if (!validRoll(roll) || !validRoll(with)) return false;
        if (!have(roll)) return false;

        use(roll);
        addOne(with);

        return true;
    }
}
