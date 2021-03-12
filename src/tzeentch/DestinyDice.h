/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Dice.h>

namespace Tzeentch {

    const int MAX_DESTINY_DICE = 9;

    class DestinyDice final {
    public:
        DestinyDice();

        ~DestinyDice() = default;

        void initialize();

        bool addOne(int roll);

        bool add(int numRolls);

        bool replace(int roll, int with);

        int use(int roll);

        void clear();

        bool have(int roll) const {
            if (validRoll(roll))
                return (m_dice.m_distribution[roll] >= 0);
            return false;
        }

        int total() const { return m_dice.rollsGE(1); }

        int count(int roll) const {
            if (validRoll(roll))
                return m_dice.m_distribution[roll];
            return 0;
        }

        const Dice::RollResult &getAll() const { return m_dice; }

    protected:

        bool validRoll(int roll) const { return ((roll > 0) && (roll <= 6)); }

    private:

        Dice::RollResult m_dice;
    };
}
