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
#include "ossiarch/OssiarchBonereaperBase.h"
#include "kharadron/KharadronBase.h"
#include "slavestodarkness/SlavesToDarkness.h"
#include "idonethdeepkin/IdonethDeepkin.h"
#include "bonesplitterz/Bonesplitterz.h"
#include "seraphon/Seraphon.h"
#include "nurgle/Nurgle.h"

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

#include "tzeentch/TzeentchBase.h"

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

#include "freeagents/GotrekGurnisson.h"

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
    OssiarchBonereapers::Init();
    KharadronOverlords::Init();
    Tzeentch::Init();
    SlavesToDarkness::Init();
    IdonethDeepkin::Init();
    Bonesplitterz::Init();
    Seraphon::Init();
    Nurgle::Init();

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

    FreeAgent::GotrekGurnisson::Init();
}
