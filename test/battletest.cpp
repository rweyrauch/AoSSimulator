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

    auto ballista0 = std::make_shared<StormcastEternals::CelestarBallista>();
    auto ballista1 = std::make_shared<StormcastEternals::CelestarBallista>();
    auto ballista2 = std::make_shared<StormcastEternals::CelestarBallista>();
    auto lordOrdinator = std::make_shared<StormcastEternals::LordOrdinator>();

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

    auto alarielle = std::make_shared<Sylvaneth::Alarielle>();
    ok = alarielle->configure(Sylvaneth::Lore::None);
    ASSERT_TRUE(ok);

    ballista0->setShootingTarget(alarielle.get());
    ballista1->setShootingTarget(alarielle.get());
    ballista2->setShootingTarget(alarielle.get());

    auto roster1 = std::make_shared<Roster>(PlayerId::Red);
    auto roster2 = std::make_shared<Roster>(PlayerId::Blue);
    roster1->addUnit(ballista0);
    roster1->addUnit(ballista1);
    roster1->addUnit(ballista2);
    roster1->addUnit(lordOrdinator);

    roster2->addUnit(alarielle);

    auto player1 = std::make_shared<Player>(PlayerId::Red);
    auto player2 = std::make_shared<Player>(PlayerId::Blue);
    player1->setRoster(roster1);
    player2->setRoster(roster2);

    battle.addPlayers(player1, player2);

    battle.setNumRounds(5);
    battle.start(PlayerId::Red);

    while (!battle.done())
    {
        battle.simulate();

        battle.next();
    }
}
