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
    Weapon starblade(Weapon::Type::Melee, "Starblade", 1, 4, 3, 3, -1, 1);
    Weapon halberd(Weapon::Type::Melee, "Castellant's Halberd", 2, 3, 3, 3, -1, 2);
    Weapon stave(Weapon::Type::Melee, "Redemption Stave", 2, 4, 3, 3, -1, RAND_D3);
    Weapon bowStave(Weapon::Type::Melee, "Bow Stave", 1, 2, 3, 4, 0, 1);
    Weapon shockBoltBow(Weapon::Type::Missile, "Shockbolt Bow", 24, 1, 3, 3, -1, 1);
    Weapon stormshoal(Weapon::Type::Melee, "Stormshoal", 3, RAND_2D6, 4, 4, 0, 1);

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
            auto hits = w.rollToHit(0, NoRerolls, 0);
            auto totalWounds = w.rollToWound(hits.numHits, 0, NoRerolls);
            auto totalDamage = totalWounds.numWoundingHit * w.damage();
            if (hits.numHits > 0)
            {
                if (totalWounds.numWoundingHit > 0)
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
                ASSERT_EQ(totalWounds.numWoundingHit, 0);
                ASSERT_EQ(totalDamage, 0);
            }
        }
    }

    // computed strength
    std::cout << "Weapon Strengths: " << std::endl;
    for (auto w : weapons)
    {
        std::cout << w.name() << " : " << w.strength() << std::endl;
    }
}