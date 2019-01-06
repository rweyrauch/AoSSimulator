/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include "WarhammerSim.h"
#include "Weapon.h"

TEST(Weapon, Attacks)
{
    Weapon starblade("Starblade", 1, 4, 3, 3, -1, 1);
    Weapon halberd("Castellant's Halberd", 2, 3, 3, 3, -1, 2);
    Weapon stave("Redemption Stave", 2, 4, 3, 3, -1, RAND_D3);
    Weapon bowStave("Bow Stave", 1, 2, 3, 4, 0, 1);
    Weapon shockBoltBow("Shockbolt Bow", 24, 1, 3, 3, -1, 1);
    Weapon stormshoal("Stormshoal", 3, RAND_2D6, 4, 4, 0, 1);

    shockBoltBow.setHitsPerAttack(RAND_D6);

    std::vector<Weapon> weapons;
    weapons.push_back(starblade);
    weapons.push_back(halberd);
    weapons.push_back(stave);
    weapons.push_back(bowStave);
    weapons.push_back(shockBoltBow);
    weapons.push_back(stormshoal);

    for (auto i = 0; i < 1000; i++)
    {
        for (auto w : weapons)
        {
            auto numHits = w.rollToHit(0, NoRerolls, 0, NoExtraHits);
            auto numWounds = w.rollToWound(numHits, 0, NoRerolls);
            auto totalDamage = numWounds * w.damage();
            if (numHits > 0)
            {
                if (numWounds > 0)
                {
                    ASSERT_GT(totalDamage, 0);
                }
                else
                {
                    ASSERT_EQ(totalDamage, 0);
                }
            }
            else
            {
                ASSERT_EQ(numWounds, 0);
                ASSERT_EQ(totalDamage, 0);
            }
        }
    }
}