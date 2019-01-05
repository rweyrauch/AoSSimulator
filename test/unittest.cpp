/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include "WarhammerSim.h"
#include "Unit.h"

TEST(Unit, Combat)
{
    Unit liberators("Liberators", 5, 2, 7, 4, false);
    Weapon warhammer("Warhammer", 1, 2, 4, 3, 0, 1);

    Model troop(40, liberators.wounds());
    Model prime(40, liberators.wounds());
    prime.addMeleeWeapon(warhammer);
    troop.addMeleeWeapon(warhammer);

    liberators.addModel(prime);
    for (int i = 0; i < 4; i++)
    {
        liberators.addModel(troop);
    }

    Unit bloodreavers("Bloodreavers", 6, 1, 5, 6, false);
    Weapon reaverBlades("Reaver Blades", 1, 1, 4, 4, 0, 1);
    Weapon chieftanBlades("Reaver Blades", 1, 2, 4, 4, 0, 1);
    Model reaver(32, bloodreavers.wounds());
    reaver.addMeleeWeapon(reaverBlades);
    Model chieftan(32, bloodreavers.wounds());
    chieftan.addMeleeWeapon(chieftanBlades);

    bloodreavers.addModel(chieftan);
    for (int i = 0; i < 9; i++)
    {
        bloodreavers.addModel(reaver);
    }
    int initialNumBloodreavers = bloodreavers.remainingModels();

    liberators.beginTurn();
    bloodreavers.beginTurn();

    liberators.combat(-1, bloodreavers);

    auto numModels = bloodreavers.remainingModels();
    if (numModels != initialNumBloodreavers)
    {
        std::cout << initialNumBloodreavers - numModels << " Bloodreavers were slain." << std::endl;
        int numFleeing = bloodreavers.battleshock(0);
        if (numFleeing > 0)
        {
            std::cout << numFleeing << " Bloodreavers failed battleshock." << std::endl;
        }
    }
}

TEST(Unit, Shoot)
{
}