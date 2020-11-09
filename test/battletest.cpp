/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <AgeOfSigmarSim.h>
#include <Unit.h>
#include <stormcast/Liberators.h>
#include <khorne/Bloodreavers.h>
#include <sylvaneth/Alarielle.h>
#include <stormcast/CelestarBallista.h>
#include <stormcast/LordOrdinator.h>
#include <Battle.h>

TEST(Battle, BallistaVsAlarielle)
{
    Battle battle;

    auto ballista0 = new StormcastEternals::CelestarBallista();
    auto ballista1 = new StormcastEternals::CelestarBallista();
    auto ballista2 = new StormcastEternals::CelestarBallista();
    auto lordOrdinator = new StormcastEternals::LordOrdinator();

    bool ok = lordOrdinator->configure(StormcastEternals::LordOrdinator::Astral_Hammers);
    ASSERT_TRUE(ok);

    ok = ballista0->configure();
    ASSERT_TRUE(ok);
    ballista0->addKeyword(ASTRAL_TEMPLARS);

    ok = ballista1->configure();
    ASSERT_TRUE(ok);
    ballista1->addKeyword(ASTRAL_TEMPLARS);

    ok = ballista2->configure();
    ASSERT_TRUE(ok);
    ballista2->addKeyword(ASTRAL_TEMPLARS);

    // apply Lord-Ordinator buffs
    //ballista0->buffToHitMissile(1);
    //ballista1->buffToHitMissile(1);
    //ballista2->buffToHitMissile(1);

    auto alarielle = new Sylvaneth::Alarielle();
    ok = alarielle->configure(Sylvaneth::Lore::None);
    ASSERT_TRUE(ok);

    ballista0->setShootingTarget(alarielle);
    ballista1->setShootingTarget(alarielle);
    ballista2->setShootingTarget(alarielle);

    Roster roster1(PlayerId::Red), roster2(PlayerId::Blue);
    roster1.addUnit(ballista0);
    roster1.addUnit(ballista1);
    roster1.addUnit(ballista2);
    roster1.addUnit(lordOrdinator);

    roster2.addUnit(alarielle);

    Player player1(PlayerId::Red), player2(PlayerId::Blue);
    player1.setRoster(&roster1);
    player2.setRoster(&roster2);

    battle.addPlayers(&player1, &player2);

    battle.setNumRounds(5);
    battle.start(PlayerId::Red);

    while (!battle.done())
    {
        battle.simulate();

        battle.next();
    }
}
