/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <AgeOfSigmarSim.h>
#include <stormcast/KnightIncantor.h>
#include <stormcast/Liberators.h>
#include <Spell.h>
#include <spells/LoreOfTheStorm.h>
#include <spells/GloomspiteSpells.h>
#include <spells/StormcastSpells.h>
#include <Roster.h>
#include <Board.h>


TEST(Spells, ArcaneBolt)
{
    auto caster = std::make_unique<StormcastEternals::KnightIncantor>();
    caster->configure(LoreOfTheStorm::None, LoreOfInvigoration::None);
    caster->setPosition(Math::Point3(0,0,0), Math::Vector3(1,0,0));

    auto target = std::make_unique<StormcastEternals::Liberators>();
    target->configure(5, StormcastEternals::Liberators::Warhammer, false, 0, 0);
    target->setPosition(Math::Point3(10,0,0), Math::Vector3(-1,0,0));

    auto spell = std::unique_ptr<Spell>(CreateArcaneBolt(caster.get()));

    for (auto i = 0; i < 10; i++)
    {
        auto ok = spell->cast(target.get(), 1);

        if (ok == Spell::Success)
        {
            ASSERT_LT(target->remainingWounds(), target->initialModels() * target->wounds());
        }
        target->restore();
    }

    // Move target out of range
    target->setPosition(Math::Point3(22,0,0), Math::Vector3(-1,0,0));
    auto ok = spell->cast(target.get(), 1);
    ASSERT_EQ(ok, Spell::Failed);
}

TEST(Spells, PrimeElectrids)
{
    auto caster = std::make_unique<StormcastEternals::KnightIncantor>();
    caster->configure(LoreOfTheStorm::None, LoreOfInvigoration::None);
    caster->setPosition(Math::Point3(0,0,0), Math::Vector3(1,0,0));

    auto target = std::make_unique<StormcastEternals::Liberators>();
    target->configure(5, StormcastEternals::Liberators::Warhammer, false, 0, 0);
    target->setPosition(Math::Point3(12,0,0), Math::Vector3(-1,0,0));

    auto spell = std::unique_ptr<Spell>(CreatePrimeElectrids(caster.get()));

    for (auto i = 0; i < 10; i++)
    {
        auto ok = spell->cast(target.get(), 1);

        if (ok == Spell::Success)
        {
            ASSERT_LT(target->remainingWounds(), target->initialModels() * target->wounds());
        }

        target->restore();
    }

    // Move target out of range
    target->setPosition(Math::Point3(22,0,0), Math::Vector3(-1,0,0));
    auto ok = spell->cast(target.get(), 1);
    ASSERT_EQ(ok, Spell::Failed);
}

TEST(Spells, Unbind)
{
    auto caster = std::make_shared<StormcastEternals::KnightIncantor>();
    caster->configure(LoreOfTheStorm::None, LoreOfInvigoration::None);
    caster->setPosition(Math::Point3(0,0,0), Math::Vector3(1,0,0));

    auto red = new Roster(PlayerId::Red);
    red->addUnit(caster);

    auto target = std::make_shared<StormcastEternals::KnightIncantor>();
    target->configure(LoreOfTheStorm::None, LoreOfInvigoration::None);
    target->setPosition(Math::Point3(10,0,0), Math::Vector3(-1,0,0));

    auto blue = new Roster(PlayerId::Blue);
    blue->addUnit(target);

    Board::Instance()->addRosters(red, blue);

    auto spell = std::unique_ptr<Spell>(CreateArcaneBolt(caster.get()));

    for (auto i = 0; i < 10; i++)
    {
       auto ok = spell->cast(target.get(), 1);

        if (ok == Spell::Success)
        {
            ASSERT_LT(target->remainingWounds(), target->initialModels() * target->wounds());
        }
        else if (ok == Spell::Unbound)
        {
            SimLog(Verbosity::Narrative, "Spell was unbound.\n");
        }
        target->restore();
    }

    // Move target out of range
    target->setPosition(Math::Point3(22,0,0), Math::Vector3(-1,0,0));
    auto ok = spell->cast(target.get(), 1);
    ASSERT_EQ(ok, Spell::Failed);

    delete blue; delete red;
}
