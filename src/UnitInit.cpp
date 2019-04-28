/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/Chainghasts.h>
#include "WarhammerSim.h"

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

#include "sylvaneth/Alarielle.h"
#include "sylvaneth/KurnothHunters.h"
#include "sylvaneth/SpiritOfDurthu.h"
#include "sylvaneth/Dryads.h"
#include "sylvaneth/TreeRevenants.h"
#include "sylvaneth/SpiteRevenants.h"
#include "sylvaneth/DrychaHamadreth.h"
#include "sylvaneth/Treelord.h"
#include "sylvaneth/TreelordAncient.h"
#include "sylvaneth/Branchwraith.h"
#include "sylvaneth/Branchwych.h"

#include "idonethdeepkin/NamartiThralls.h"
#include "idonethdeepkin/NamartiReavers.h"
#include "idonethdeepkin/IshlaenGuard.h"
#include "idonethdeepkin/MorrsarrGuard.h"
#include "idonethdeepkin/AkhelianLeviadon.h"
#include "idonethdeepkin/AkhelianKing.h"
#include "idonethdeepkin/AkhelianAllopexes.h"

#include "beastsofchaos/Gors.h"
#include "beastsofchaos/Ghorgon.h"
#include "beastsofchaos/Ungors.h"
#include "beastsofchaos/Bestigors.h"
#include "beastsofchaos/Bullgors.h"
#include "beastsofchaos/Cygor.h"
#include "beastsofchaos/Centigors.h"
#include "beastsofchaos/Doombull.h"
#include "beastsofchaos/Chimera.h"

#include "slaanesh/Fiends.h"
#include "slaanesh/Seekers.h"
#include "slaanesh/Daemonettes.h"
#include "slaanesh/KeeperOfSecrets.h"
#include "slaanesh/Hellstriders.h"
#include "slaanesh/ShalaxiHelbane.h"
#include "slaanesh/BladebringerExaltedChariot.h"
#include "slaanesh/BladebringerHellflayer.h"
#include "slaanesh/BladebringerSeekerChariot.h"
#include "slaanesh/ExaltedChariot.h"
#include "slaanesh/Hellflayer.h"
#include "slaanesh/SeekerChariots.h"
#include "slaanesh/TheMasque.h"
#include "slaanesh/Viceleader.h"
#include "slaanesh/InfernalEnrapturess.h"

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

    Sylvaneth::Alarielle::Init();
    Sylvaneth::KurnothHunters::Init();
    Sylvaneth::SpiritOfDurthu::Init();
    Sylvaneth::Dryads::Init();
    Sylvaneth::TreeRevenants::Init();
    Sylvaneth::SpiteRevenants::Init();
    Sylvaneth::DrychaHamadreth::Init();
    Sylvaneth::Treelord::Init();
    Sylvaneth::TreelordAncient::Init();
    Sylvaneth::Branchwraith::Init();
    Sylvaneth::Branchwych::Init();

    IdonethDeepkin::NamartiThralls::Init();
    IdonethDeepkin::NamartiReavers::Init();
    IdonethDeepkin::AkhelianMorrsarrGuard::Init();
    IdonethDeepkin::AkhelianIshlaenGuard::Init();
    IdonethDeepkin::AkhelianLeviadon::Init();
    IdonethDeepkin::AkhelianKing::Init();
    IdonethDeepkin::AkhelianAllopexes::Init();

    BeastsOfChaos::Gors::Init();
    BeastsOfChaos::Ghorgon::Init();
    BeastsOfChaos::Bestigors::Init();
    BeastsOfChaos::Bullgors::Init();
    BeastsOfChaos::Ungors::Init();
    BeastsOfChaos::Cygor::Init();
    BeastsOfChaos::Centigors::Init();
    BeastsOfChaos::Doombull::Init();
    BeastsOfChaos::Chimera::Init();

    Slaanesh::Daemonettes::Init();
    Slaanesh::Fiends::Init();
    Slaanesh::Seekers::Init();
    Slaanesh::KeeperOfSecrets::Init();
    Slaanesh::Hellstriders::Init();
    Slaanesh::ShalaxiHelbane::Init();
    Slaanesh::BladebringerOnExaltedChariot::Init();
    Slaanesh::BladebringerOnHellflayer::Init();
    Slaanesh::BladebringerOnSeekerChariot::Init();
    Slaanesh::ExaltedChariot::Init();
    Slaanesh::Hellflayer::Init();
    Slaanesh::SeekerChariots::Init();
    Slaanesh::TheMasque::Init();
    Slaanesh::ViceleaderHeraldOfSlaanesh::Init();
    Slaanesh::InfernalEnrapturess::Init();
    
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
}
