/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <WarhammerSim.h>
#include <Unit.h>
#include <stormcast/Liberators.h>
#include <khorne/Bloodreavers.h>
#include <sylvaneth/Alarielle.h>
#include <stormcast/CelestarBallista.h>
#include <stormcast/LordOrdinator.h>

TEST(Unit, Combat)
{
    CustomUnit liberators("Liberators", 5, 2, 7, 4, false);
    Weapon warhammer("Warhammer", 1, 2, 4, 3, 0, 1);
    Weapon warhammerPrime("Warhammer", 1, 3, 4, 3, 0, 1);

    Model troop(40, liberators.wounds());
    Model prime(40, liberators.wounds());
    prime.addMeleeWeapon(warhammerPrime);
    troop.addMeleeWeapon(warhammer);

    liberators.addModel(prime);
    for (int i = 0; i < 4; i++)
    {
        liberators.addModel(troop);
    }
    int initialNumLibs = liberators.remainingModels();

    CustomUnit bloodreavers("Bloodreavers", 6, 1, 5, 6, false);
    Weapon reaverBlades("Reaver Blades", 1, 1, 4, 4, 0, 1);
    Weapon chieftanBlades("Reaver Blades", 1, 2, 4, 4, 0, 1);
    Model reaver(32, bloodreavers.wounds());
    reaver.addMeleeWeapon(reaverBlades);
    Model chieftan(32, bloodreavers.wounds());
    chieftan.addMeleeWeapon(chieftanBlades);

    bloodreavers.addModel(chieftan);
    for (int i = 0; i < 19; i++)
    {
        bloodreavers.addModel(reaver);
    }
    int initialNumBloodreavers = bloodreavers.remainingModels();

    for (auto i = 0; i < 2; i++)
    {
        std::cout << liberators.remainingModels() << " Liberators at the start of round " << i << std::endl;
        std::cout << bloodreavers.remainingModels() << " Bloodreavers at the start of round " << i << std::endl;

        liberators.beginTurn();
        bloodreavers.beginTurn();

        auto totalDamage = liberators.combat(-1, bloodreavers);
        std::cout << "   Liberators inflicted " << totalDamage << " damage onto Bloodreavers. " <<
            bloodreavers.remainingModels() << " Bloodreavers remain." << std::endl;

        totalDamage = bloodreavers.combat(-1, liberators);
        std::cout << "   Bloodreavers inflicted " << totalDamage << " damage onto Liberators. " <<
            liberators.remainingModels() << " Liberators remain." << std::endl;

        auto numModels = bloodreavers.remainingModels();
        if (numModels != initialNumBloodreavers)
        {
            std::cout << "   " << initialNumBloodreavers - numModels << " Bloodreavers were slain."
                      << std::endl;
            int numFleeing = bloodreavers.battleshock(0);
            std::cout << "   " << numFleeing << " Bloodreavers failed battleshock." << std::endl;

        }
        numModels = liberators.remainingModels();
        if (numModels != initialNumLibs)
        {
            std::cout << "   " << initialNumLibs - numModels << " Liberators were slain." << std::endl;
            int numFleeing = liberators.battleshock(0);
            std::cout << "   " <<  numFleeing << " Liberators failed battleshock." << std::endl;
        }
    }
    std::cout << liberators.remainingModels() << " Liberators at the end of the battle." << std::endl;
    std::cout << bloodreavers.remainingModels() << " Bloodreavers at the end of the battle." << std::endl;
}

TEST(Unit, Shoot)
{
}

TEST(Unit, LiberatorsVsBloodreavers)
{
    StormcastEternals::Liberators liberators;
    Khorne::Bloodreavers bloodreavers;

    bool ok = liberators.configure(5, StormcastEternals::Liberators::Warhammer, 0, 0);
    ASSERT_TRUE(ok);

    int initialNumLibs = liberators.remainingModels();
    ASSERT_EQ(initialNumLibs, 5);

    ok = bloodreavers.configure(10, Khorne::Bloodreavers::ReaverBlades, false, false);
    ASSERT_TRUE(ok);

    int initialNumBloodreavers = bloodreavers.remainingModels();
    ASSERT_EQ(initialNumBloodreavers, 10);

    for (auto i = 0; i < 2; i++)
    {
        std::cout << liberators.remainingModels() << " Liberators at the start of round " << i << std::endl;
        std::cout << bloodreavers.remainingModels() << " Bloodreavers at the start of round " << i << std::endl;

        liberators.beginTurn();
        bloodreavers.beginTurn();

        auto totalDamage = liberators.combat(-1, bloodreavers);
        std::cout << "   Liberators inflicted " << totalDamage << " damage onto Bloodreavers. " <<
                  bloodreavers.remainingModels() << " Bloodreavers remain." << std::endl;

        totalDamage = bloodreavers.combat(-1, liberators);
        std::cout << "   Bloodreavers inflicted " << totalDamage << " damage onto Liberators. " <<
                  liberators.remainingModels() << " Liberators remain." << std::endl;

        auto numModels = bloodreavers.remainingModels();
        std::cout << "   " << initialNumBloodreavers - numModels << " Bloodreavers were slain." << std::endl;
        int numFleeing = bloodreavers.battleshock(0);
        std::cout << "   " << numFleeing << " Bloodreavers failed battleshock." << std::endl;

        numModels = liberators.remainingModels();
        std::cout << "   " << initialNumLibs - numModels << " Liberators were slain." << std::endl;
        numFleeing = liberators.battleshock(0);
        std::cout << "   " <<  numFleeing << " Liberators failed battleshock." << std::endl;
    }
    std::cout << liberators.remainingModels() << " Liberators at the end of the battle." << std::endl;
    std::cout << bloodreavers.remainingModels() << " Bloodreavers at the end of the battle." << std::endl;
}

TEST(Unit, BallistaVsAlarielle)
{
    StormcastEternals::CelestarBallista ballista0, ballista1, ballista2;
    StormcastEternals::LordOrdinator lordOrdinator;

    bool ok = lordOrdinator.configure(StormcastEternals::LordOrdinator::AstralHammers);
    ASSERT_TRUE(ok);

    const bool singleShot = false;
    ok = ballista0.configure(singleShot);
    ASSERT_TRUE(ok);
    ballista0.addKeyword(ASTRAL_TEMPLARS);

    ok = ballista1.configure(singleShot);
    ASSERT_TRUE(ok);
    ballista1.addKeyword(ASTRAL_TEMPLARS);

    ok = ballista2.configure(singleShot);
    ASSERT_TRUE(ok);
    ballista2.addKeyword(ASTRAL_TEMPLARS);

    // apply Lord-Ordinator buffs
    ballista0.buffToHitMissile(1);
    ballista1.buffToHitMissile(1);
    ballista2.buffToHitMissile(1);

    Sylvaneth::Alarielle alarielle;
    ok = alarielle.configure();
    ASSERT_TRUE(ok);

    for (auto i = 0; i < 5; i++)
    {
        std::cout << "Alarielle has " << alarielle.remainingWounds() << " wounds remaining." << std::endl;
        alarielle.beginTurn();
        ballista0.beginTurn();
        ballista1.beginTurn();
        ballista2.beginTurn();
        lordOrdinator.beginTurn();

        auto totalDamage = ballista0.shooting(-1, alarielle);
        std::cout << "Ballista0 inflicted " << totalDamage << " wounds on Alarielle." << std::endl;
        totalDamage = ballista1.shooting(-1, alarielle);
        std::cout << "Ballista1 inflicted " << totalDamage << " wounds on Alarielle." << std::endl;
        totalDamage = ballista2.shooting(-1, alarielle);
        std::cout << "Ballista2 inflicted " << totalDamage << " wounds on Alarielle." << std::endl;

        if (alarielle.remainingWounds() <= 0)
        {
            std::cout << "Alarielle has been slain." << std::endl;
            break;
        }
    }
    std::cout << "Alarielle has " << alarielle.remainingWounds() << " wounds remaining after the battle." << std::endl;
}