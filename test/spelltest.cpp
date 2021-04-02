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
#include <stormcast/LoreOfTheStorm.h>
#include <gloomspitegitz/GloomspiteSpells.h>
#include <stormcast/StormcastSpells.h>
#include <Roster.h>
#include <Board.h>


TEST(Spells, ArcaneBolt)
{
    auto caster = std::make_unique<StormcastEternals::KnightIncantor>(StormcastEternals::Stormhost::Astral_Templars, StormcastEternals::Lore::Lightning_Blast,
                                                                      StormcastEternals::CommandTrait::None, StormcastEternals::Artefact::None, false);
    caster->deploy(Math::Point3(0,0,0), Math::Vector3(1,0,0));

    auto target = std::make_unique<StormcastEternals::Liberators>();
    target->configure(5, StormcastEternals::Liberators::Warhammer, false, 0, 0);
    target->deploy(Math::Point3(10,0,0), Math::Vector3(-1,0,0));

    auto spell = std::unique_ptr<Spell>(CreateArcaneBolt(caster.get()));

    for (auto i = 0; i < 10; i++)
    {
        auto ok = spell->cast(target.get(), 1);

        if (ok == Spell::Result::Success)
        {
            ASSERT_LT(target->remainingWounds(), target->initialModels() * target->wounds());
        }
        target->restore();
    }

    // Move target out of range
    target->deploy(Math::Point3(22,0,0), Math::Vector3(-1,0,0));
    auto ok = spell->cast(target.get(), 1);
    ASSERT_EQ(ok, Spell::Result::Failed);
}

TEST(Spells, PrimeElectrids)
{
    auto caster = std::make_unique<StormcastEternals::KnightIncantor>(StormcastEternals::Stormhost::Astral_Templars, StormcastEternals::Lore::Lightning_Blast,
                                                                      StormcastEternals::CommandTrait::None, StormcastEternals::Artefact::None, false);
    caster->deploy(Math::Point3(0,0,0), Math::Vector3(1,0,0));

    auto target = std::make_unique<StormcastEternals::Liberators>();
    target->configure(5, StormcastEternals::Liberators::Warhammer, false, 0, 0);
    target->deploy(Math::Point3(12,0,0), Math::Vector3(-1,0,0));

    auto spell = std::unique_ptr<Spell>(CreatePrimeElectrids(caster.get()));

    for (auto i = 0; i < 10; i++)
    {
        auto ok = spell->cast(target.get(), 1);

        if (ok == Spell::Result::Success)
        {
            ASSERT_LT(target->remainingWounds(), target->initialModels() * target->wounds());
        }

        target->restore();
    }

    // Move target out of range
    target->deploy(Math::Point3(22,0,0), Math::Vector3(-1,0,0));
    auto ok = spell->cast(target.get(), 1);
    ASSERT_EQ(ok, Spell::Result::Failed);
}

TEST(Spells, Unbind)
{
    auto caster = std::make_shared<StormcastEternals::KnightIncantor>(StormcastEternals::Stormhost::Astral_Templars, StormcastEternals::Lore::Lightning_Blast,
                                                                      StormcastEternals::CommandTrait::None, StormcastEternals::Artefact::None, false);
    caster->deploy(Math::Point3(0,0,0), Math::Vector3(1,0,0));

    auto red = std::make_shared<Roster>(PlayerId::Red);
    red->addUnit(caster);

    auto target = std::make_shared<StormcastEternals::KnightIncantor>(StormcastEternals::Stormhost::Astral_Templars, StormcastEternals::Lore::Lightning_Blast,
                                                                      StormcastEternals::CommandTrait::None, StormcastEternals::Artefact::None, false);
    target->deploy(Math::Point3(10,0,0), Math::Vector3(-1,0,0));

    auto blue = std::make_shared<Roster>(PlayerId::Blue);
    blue->addUnit(target);

    Board::Instance()->addRosters(red, blue);

    auto spell = std::unique_ptr<Spell>(CreateArcaneBolt(caster.get()));

    for (auto i = 0; i < 10; i++)
    {
       auto ok = spell->cast(target.get(), 1);

        if (ok == Spell::Result::Success)
        {
            ASSERT_LT(target->remainingWounds(), target->initialModels() * target->wounds());
        }
        else if (ok == Spell::Result::Unbound)
        {
            PLOGD.printf("Spell was unbound.\n");
        }
        target->restore();
    }

    // Move target out of range
    target->deploy(Math::Point3(22,0,0), Math::Vector3(-1,0,0));
    auto ok = spell->cast(target.get(), 1);
    ASSERT_EQ(ok, Spell::Result::Failed);
}
