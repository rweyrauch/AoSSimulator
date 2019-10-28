/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "AgeOfSigmarSim.h"

#include "stormcast/StormcastEternals.h"
#include "gloomspitegitz/GloomspiteGitzBase.h"
#include "fec/FleshEaterCourts.h"
#include "dok/DaughterOfKhaine.h"
#include "nighthaunt/Nighthaunt.h"
#include "skaven/Skaventide.h"
#include "dispossessed/Dispossessed.h"
#include "ironjawz/Ironjawz.h"
#include "khorne/KhorneBase.h"
#include "wanderers/Wanderer.h"
#include "fyreslayers/Fyreslayer.h"
#include "slaanesh/SlaaneshBase.h"
#include "beastsofchaos/BeastsOfChaosBase.h"
#include "sylvaneth/SylvanethBase.h"
#include "citiesofsigmar/CitiesOfSigmar.h"
#include "mawtribes/MawtribesBase.h"

#include "idonethdeepkin/NamartiThralls.h"
#include "idonethdeepkin/NamartiReavers.h"
#include "idonethdeepkin/IshlaenGuard.h"
#include "idonethdeepkin/MorrsarrGuard.h"
#include "idonethdeepkin/AkhelianLeviadon.h"
#include "idonethdeepkin/AkhelianKing.h"
#include "idonethdeepkin/AkhelianAllopexes.h"

#include "nurgle/LordOfPlagues.h"
#include "nurgle/Plaguebearers.h"
#include "nurgle/PutridBlightkings.h"
#include "nurgle/BeastsOfNurgle.h"
#include "nurgle/TheGlottkin.h"
#include "nurgle/PlagueDrones.h"
#include "nurgle/PusgoyleBlightlords.h"

#include "death/BlackKnights.h"
#include "death/BloodKnights.h"
#include "death/DireWolves.h"
#include "death/GraveGuard.h"
#include "death/SkeletonWarriors.h"
#include "death/Vargheists.h"
#include "death/Zombies.h"
#include "death/WightKingTombBlade.h"
#include "death/WightKingBlackAxe.h"
#include "death/Necromancer.h"
#include "death/MorghastArchai.h"
#include "death/MorghastHarbingers.h"

#include "tzeentch/KairicAcolytes.h"
#include "tzeentch/PinkHorrors.h"
#include "tzeentch/TzaangorEnlightened.h"
#include "tzeentch/Tzaangors.h"
#include "tzeentch/FlamersOfTzeentch.h"

#include "eldritchcouncil/Swordmasters.h"
#include "eldritchcouncil/Loremaster.h"
#include "eldritchcouncil/Archmage.h"
#include "eldritchcouncil/ArchmageOnDragon.h"

#include "greenskinz/RogueIdol.h"
#include "greenskinz/Orruks.h"
#include "greenskinz/OrrukBoarboys.h"
#include "greenskinz/OrrukBoarChariots.h"
#include "greenskinz/OrrukGreatShaman.h"
#include "greenskinz/OrrukWarboss.h"
#include "greenskinz/OrrukWarbossWyvern.h"

#include "seraphon/DreadSaurian.h"
#include "seraphon/SaurusGuard.h"
#include "seraphon/SaurusKnights.h"
#include "seraphon/SaurusWarriors.h"
#include "seraphon/Skinks.h"

#include "everchosen/Archaon.h"

#include "slavestodarkness/ChaosChosen.h"
#include "slavestodarkness/ChaosKnights.h"
#include "slavestodarkness/ChaosMarauders.h"
#include "slavestodarkness/ChaosWarriors.h"
#include "slavestodarkness/CorvusCabal.h"
#include "slavestodarkness/CypherLords.h"
#include "slavestodarkness/Furies.h"
#include "slavestodarkness/IronGolems.h"
#include "slavestodarkness/Raptoryx.h"
#include "slavestodarkness/Slambo.h"
#include "slavestodarkness/SplinteredFang.h"
#include "slavestodarkness/TheUnmade.h"
#include "slavestodarkness/UntamedBeasts.h"

#include "freeagents/GotrekGurnisson.h"

#include "bonesplitterz/BoarboyManiaks.h"
#include "bonesplitterz/ManiakWeirdnob.h"
#include "bonesplitterz/OrrukArrowBoys.h"
#include "bonesplitterz/OrrukMorboys.h"
#include "bonesplitterz/SavageBigBoss.h"
#include "bonesplitterz/SavageBigStabbas.h"
#include "bonesplitterz/SavageBoarboys.h"
#include "bonesplitterz/SavageOrruks.h"
#include "bonesplitterz/Wardokk.h"
#include "bonesplitterz/WurrgogProphet.h"

static Verbosity g_verbosity = Verbosity::Normal;

Verbosity GetVerbosity()
{
    return g_verbosity;
}

void SetVerbosity(Verbosity verbosity)
{
    g_verbosity = verbosity;
}

void Initialize(Verbosity verbosity)
{
    g_verbosity = verbosity;

    StormcastEternals::Init();
    GloomspiteGitz::Init();
    FleshEaterCourt::Init();
    DaughtersOfKhaine::Init();
    Nighthaunt::Init();
    Skaven::Init();
    Dispossessed::Init();
    Ironjawz::Init();
    Khorne::Init();
    Wanderers::Init();
    Fyreslayers::Init();
    Slaanesh::Init();
    BeastsOfChaos::Init();
    Sylvaneth::Init();
    CitiesOfSigmar::Init();
    OgorMawtribes::Init();

    IdonethDeepkin::NamartiThralls::Init();
    IdonethDeepkin::NamartiReavers::Init();
    IdonethDeepkin::AkhelianMorrsarrGuard::Init();
    IdonethDeepkin::AkhelianIshlaenGuard::Init();
    IdonethDeepkin::AkhelianLeviadon::Init();
    IdonethDeepkin::AkhelianKing::Init();
    IdonethDeepkin::AkhelianAllopexes::Init();

    Nurgle::LordOfPlagues::Init();
    Nurgle::Plaguebearers::Init();
    Nurgle::PutridBlightkings::Init();
    Nurgle::BeastsOfNurgle::Init();
    Nurgle::TheGlottkin::Init();
    Nurgle::PlagueDrones::Init();
    Nurgle::PusgoyleBlightlords::Init();

    Death::BlackKnights::Init();
    Death::BloodKnights::Init();
    Death::DireWolves::Init();
    Death::GraveGuard::Init();
    Death::SkeletonWarriors::Init();
    Death::Vargheists::Init();
    Death::Zombies::Init();
    Death::WightKingWithBlackAxe::Init();
    Death::WightKingWithBalefulTombBlade::Init();
    Death::Necromancer::Init();
    Death::MorghastArchai::Init();
    Death::MorghastHarbingers::Init();

    Tzeentch::KairicAcolytes::Init();
    Tzeentch::PinkHorrors::Init();
    Tzeentch::TzaangorEnlightened::Init();
    Tzeentch::Tzaangors::Init();
    Tzeentch::FlamersOfTzeentch::Init();

    EldritchCouncil::Swordmasters::Init();
    EldritchCouncil::Loremaster::Init();
    EldritchCouncil::Archmage::Init();
    EldritchCouncil::ArchmageOnDragon::Init();

    Greenskinz::Orruks::Init();
    Greenskinz::OrrukBoarboys::Init();
    Greenskinz::OrrukBoarChariots::Init();
    Greenskinz::OrrukGreatShaman::Init();
    Greenskinz::OrrukWarboss::Init();
    Greenskinz::OrrukWarbossOnWyvern::Init();
    Greenskinz::RogueIdol::Init();

    Seraphon::DreadSaurian::Init();
    Seraphon::SaurusGuard::Init();
    Seraphon::SaurusKnights::Init();
    Seraphon::SaurusWarriors::Init();
    Seraphon::Skinks::Init();

    Everchosen::Archaon::Init();

    SlavesToDarkness::ChaosChosen::Init();
    SlavesToDarkness::ChaosKnights::Init();
    SlavesToDarkness::ChaosMarauders::Init();
    SlavesToDarkness::ChaosWarriors::Init();
    SlavesToDarkness::CorvusCabal::Init();
    SlavesToDarkness::CypherLords::Init();
    SlavesToDarkness::Furies::Init();
    SlavesToDarkness::IronGolems::Init();
    SlavesToDarkness::Raptoryx::Init();
    SlavesToDarkness::Slambo::Init();
    SlavesToDarkness::SplinteredFang::Init();
    SlavesToDarkness::TheUnmade::Init();
    SlavesToDarkness::UntamedBeasts::Init();

    FreeAgent::GotrekGurnisson::Init();

    Bonesplitterz::SavageBoarboyManiaks::Init();
    Bonesplitterz::ManiakWeirdnob::Init();
    Bonesplitterz::SavageOrrukArrowboys::Init();
    Bonesplitterz::SavageOrrukMorboys::Init();
    Bonesplitterz::SavageBigBoss::Init();
    Bonesplitterz::SavageBigStabbas::Init();
    Bonesplitterz::SavageBoarboys::Init();
    Bonesplitterz::SavageOrruks::Init();
    Bonesplitterz::Wardokk::Init();
    Bonesplitterz::WurrgogProphet::Init();
}
