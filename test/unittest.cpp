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
    Weapon warhammer(Weapon::Type::Melee, "Warhammer", 1, 2, 4, 3, 0, 1);
    Weapon warhammerPrime(Weapon::Type::Melee, "Warhammer", 1, 3, 4, 3, 0, 1);

    Model troop(40, liberators.wounds());
    Model prime(40, liberators.wounds());
    prime.addMeleeWeapon(&warhammerPrime);
    troop.addMeleeWeapon(&warhammer);

    liberators.addModel(prime);
    for (int i = 0; i < 4; i++)
    {
        liberators.addModel(troop);
    }
    int initialNumLibs = liberators.remainingModels();

    CustomUnit bloodreavers("Bloodreavers", 6, 1, 5, 6, false);
    Weapon reaverBlades(Weapon::Type::Melee, "Reaver Blades", 1, 1, 4, 4, 0, 1);
    Weapon chieftanBlades(Weapon::Type::Melee, "Reaver Blades", 1, 2, 4, 4, 0, 1);
    Model reaver(32, bloodreavers.wounds());
    reaver.addMeleeWeapon(&reaverBlades);
    Model chieftan(32, bloodreavers.wounds());
    chieftan.addMeleeWeapon(&chieftanBlades);

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

        liberators.beginTurn(i);
        bloodreavers.beginTurn(i);

        int numSlain = 0;
        auto totalDamage = liberators.fight(-1, &bloodreavers, numSlain);
        std::cout << "   Liberators inflicted " << (totalDamage.normal+totalDamage.mortal) << " damage onto Bloodreavers. " <<
            bloodreavers.remainingModels() << " Bloodreavers remain." << std::endl;

        totalDamage = bloodreavers.fight(-1, &liberators, numSlain);
        std::cout << "   Bloodreavers inflicted " << (totalDamage.normal+totalDamage.mortal) << " damage onto Liberators. " <<
            liberators.remainingModels() << " Liberators remain." << std::endl;

        auto numModels = bloodreavers.remainingModels();
        if (numModels != initialNumBloodreavers)
        {
            std::cout << "   " << initialNumBloodreavers - numModels << " Bloodreavers were slain."
                      << std::endl;
            int numFleeing = bloodreavers.applyBattleshock();
            std::cout << "   " << numFleeing << " Bloodreavers failed battleshock." << std::endl;

        }
        numModels = liberators.remainingModels();
        if (numModels != initialNumLibs)
        {
            std::cout << "   " << initialNumLibs - numModels << " Liberators were slain." << std::endl;
            int numFleeing = liberators.applyBattleshock();
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

    bool ok = liberators.configure(5, StormcastEternals::Liberators::Warhammer, false, 0, 0);
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

        liberators.beginTurn(i);
        bloodreavers.beginTurn(i);

        int numSlain = 0;
        auto totalDamage = liberators.fight(-1, &bloodreavers, numSlain);
        std::cout << "   Liberators inflicted " << (totalDamage.normal+totalDamage.mortal) << " damage onto Bloodreavers. " <<
                  bloodreavers.remainingModels() << " Bloodreavers remain." << std::endl;

        totalDamage = bloodreavers.fight(-1, &liberators, numSlain);
        std::cout << "   Bloodreavers inflicted " << (totalDamage.normal+totalDamage.mortal) << " damage onto Liberators. " <<
                  liberators.remainingModels() << " Liberators remain." << std::endl;

        auto numModels = bloodreavers.remainingModels();
        std::cout << "   " << initialNumBloodreavers - numModels << " Bloodreavers were slain." << std::endl;
        int numFleeing = bloodreavers.applyBattleshock();
        std::cout << "   " << numFleeing << " Bloodreavers failed battleshock." << std::endl;

        numModels = liberators.remainingModels();
        std::cout << "   " << initialNumLibs - numModels << " Liberators were slain." << std::endl;
        numFleeing = liberators.applyBattleshock();
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

    ok = ballista0.configure();
    ASSERT_TRUE(ok);
    ballista0.addKeyword(ASTRAL_TEMPLARS);

    ok = ballista1.configure();
    ASSERT_TRUE(ok);
    ballista1.addKeyword(ASTRAL_TEMPLARS);

    ok = ballista2.configure();
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
        alarielle.beginTurn(i);
        ballista0.beginTurn(i);
        ballista1.beginTurn(i);
        ballista2.beginTurn(i);
        lordOrdinator.beginTurn(i);

        alarielle.hero(PlayerId::Red);
        std::cout << "Alarielle has " << alarielle.remainingWounds() << " wounds remaining after hero phase." << std::endl;

        int numSlain = 0;
        auto totalDamage = ballista0.shoot(-1, &alarielle, numSlain);
        std::cout << "Ballista0 inflicted " << (totalDamage.normal+totalDamage.mortal) << " wounds on Alarielle." << std::endl;
        totalDamage = ballista1.shoot(-1, &alarielle, numSlain);
        std::cout << "Ballista1 inflicted " << (totalDamage.normal+totalDamage.mortal) << " wounds on Alarielle." << std::endl;
        totalDamage = ballista2.shoot(-1, &alarielle, numSlain);
        std::cout << "Ballista2 inflicted " << (totalDamage.normal+totalDamage.mortal) << " wounds on Alarielle." << std::endl;

        if (alarielle.remainingWounds() > 0)
        {
            if (ballista0.remainingWounds() > 0)
            {
                totalDamage = alarielle.shoot(-1, &ballista0, numSlain);
                std::cout << "Alarielle inflicted " << (totalDamage.normal+totalDamage.mortal) << " wounds on Ballista0."
                          << std::endl;
            }
            else if (ballista1.remainingWounds() > 0)
            {
                totalDamage = alarielle.shoot(-1, &ballista1, numSlain);
                std::cout << "Alarielle inflicted " << (totalDamage.normal+totalDamage.mortal) << " wounds on Ballista1."
                          << std::endl;
            }
            else if (ballista2.remainingWounds() > 0)
            {
                totalDamage = alarielle.shoot(-1, &ballista2, numSlain);
                std::cout << "Alarielle inflicted " << (totalDamage.normal+totalDamage.mortal) << " wounds on Ballista2."
                          << std::endl;
            }
            else
            {
                totalDamage = alarielle.shoot(-1, &lordOrdinator, numSlain);
                std::cout << "Alarielle inflicted " << (totalDamage.normal+totalDamage.mortal) << " wounds on "
                          << lordOrdinator.name() << "." << std::endl;
            }
        }
        else
        {
            std::cout << "Alarielle has been slain." << std::endl;
            break;
        }
    }
    std::cout << "Alarielle has " << alarielle.remainingWounds() << " wounds remaining after the battle." << std::endl;
}

TEST(Unit, DamageTable)
{
    Sylvaneth::Alarielle alarielle;
    bool ok = alarielle.configure();
    ASSERT_TRUE(ok);

    Wounds wounds;
    wounds.normal = 1;
    wounds.mortal = 0;


    for (auto i = 0; i < Sylvaneth::Alarielle::WOUNDS; i++)
    {
        alarielle.applyDamage(wounds);
        std::cout << "Wound: " << alarielle.remainingWounds() << "  Move: " << alarielle.move() << std::endl;
    }
}

TEST(Unit, RegisteredUnits)
{
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        std::cout << "Unit name: " << ruip->first << std::endl;

        for (auto pip : ruip->second.m_parameters)
        {
            if (pip.m_paramType == ParamType::Integer)
            {
                std::cout << "\tParam: " << pip.m_name << " Type: Integer  Value: " << pip.m_intValue
                          << "  Min: " << pip.m_minValue << "  Max: " << pip.m_maxValue << std::endl;
            }
            else if (pip.m_paramType == ParamType::Boolean)
            {
                std::cout << "\tParam: " << pip.m_name << " Type: Boolean  Value: "
                          << (pip.m_boolValue ? "true" : "false") << std::endl;
            }
        }
    }
}

TEST(Unit, ConfigureRegisteredUnits)
{
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        std::vector<Parameter> parameters;
        for (auto pip : ruip->second.m_parameters)
        {
            Parameter param = pip;
            if ((param.m_paramType == ParamType::Integer) && (param.m_name == "numModels"))
            {
                param.m_intValue = param.m_maxValue;
            }
            parameters.push_back(param);
        }

        auto unit = UnitFactory::Create(ruip->first, parameters);
        if (unit == nullptr)
        {
            std::cerr << "Failed to create unit: " << ruip->first << std::endl;
            ASSERT_NE(unit, nullptr);
        }
    }
}