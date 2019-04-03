/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/Chainghasts.h>
#include <WarhammerSim.h>
#include <stormcast/Decimators.h>

#include "WarhammerSim.h"

#include "stormcast/Liberators.h"
#include "stormcast/CelestarBallista.h"
#include "stormcast/Evocators.h"
#include "stormcast/Judicators.h"
#include "stormcast/Sequitors.h"
#include "stormcast/LordOrdinator.h"
#include "stormcast/Retributors.h"
#include "stormcast/LordCelestant.h"
#include "stormcast/LordCelestantOnDracoth.h"
#include "stormcast/LordCelestantOnStardrake.h"
#include "stormcast/KnightIncantor.h"
#include "stormcast/KnightQuestor.h"
#include "stormcast/KnightZephyros.h"
#include "stormcast/VanguardHunters.h"
#include "stormcast/VanguardRaptorsHurricane.h"
#include "stormcast/VanguardRaptorsLongstrike.h"
#include "stormcast/Castigators.h"
#include "stormcast/VandusHammerhand.h"
#include "stormcast/GryphHounds.h"
#include "stormcast/Concussors.h"
#include "stormcast/Desolators.h"
#include "stormcast/Fulminators.h"
#include "stormcast/Tempestors.h"
#include "stormcast/EvocatorsOnDracolines.h"
#include "stormcast/Prosecutors.h"
#include "stormcast/Protectors.h"
#include "stormcast/Decimators.h"
#include "stormcast/NeaveBlacktalon.h"
#include "stormcast/GavrielSureheart.h"

#include "gloomspitegitz/BoingrotBounderz.h"
#include "gloomspitegitz/ColossalSquig.h"
#include "gloomspitegitz/LoonbossOnManglerSquigs.h"
#include "gloomspitegitz/ManglerSquigs.h"
#include "gloomspitegitz/SquigHerd.h"
#include "gloomspitegitz/SquigHoppers.h"
#include "gloomspitegitz/Stabbas.h"
#include "gloomspitegitz/Shootas.h"
#include "gloomspitegitz/RockgutTroggoths.h"
#include "gloomspitegitz/DankholdTroggoths.h"
#include "gloomspitegitz/DankholdTroggboss.h"
#include "gloomspitegitz/Skragrott.h"
#include "gloomspitegitz/Loonboss.h"
#include "gloomspitegitz/LoonbossOnGiantCaveSquig.h"
#include "gloomspitegitz/LoonbossWithGiantCaveSquig.h"
#include "gloomspitegitz/AleguzzlerGargant.h"
#include "gloomspitegitz/SquigGobba.h"
#include "gloomspitegitz/SpiderRiders.h"
#include "gloomspitegitz/FellwaterTroggoths.h"

#include "khorne/Bloodreavers.h"
#include "khorne/BloodWarriors.h"
#include "khorne/Bloodletters.h"
#include "khorne/Skullreapers.h"
#include "khorne/Wrathmongers.h"
#include "khorne/Khorgoraths.h"
#include "khorne/BloodthirsterOfUnfetteredFury.h"
#include "khorne/BloodthirsterOfInsensateRage.h"
#include "khorne/WrathOfKhorneBloodthirster.h"
#include "khorne/Skarbrand.h"
#include "khorne/Karanak.h"
#include "khorne/FleshHounds.h"
#include "khorne/Skulltaker.h"
#include "khorne/Bloodmaster.h"
#include "khorne/Bloodcrushers.h"
#include "khorne/MightySkullcrushers.h"
#include "khorne/Skullmaster.h"

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

#include "dok/WitchAelves.h"
#include "dok/BloodSisters.h"
#include "dok/KhineraiLifetakers.h"
#include "dok/SistersOfSlaughter.h"
#include "dok/KhineraiHeartrenders.h"
#include "dok/DoomfireWarlocks.h"
#include "dok/BloodStalkers.h"
#include "dok/HagQueen.h"
#include "dok/SlaughterQueen.h"
#include "dok/AvatarOfKhaine.h"
#include "dok/BloodwrackMedusa.h"

#include "idonethdeepkin/NamartiThralls.h"
#include "idonethdeepkin/NamartiReavers.h"
#include "idonethdeepkin/IshlaenGuard.h"
#include "idonethdeepkin/MorrsarrGuard.h"
#include "idonethdeepkin/AkhelianLeviadon.h"
#include "idonethdeepkin/AkhelianKing.h"
#include "idonethdeepkin/AkhelianAllopexes.h"

#include "nighthaunt/ChainraspHorde.h"
#include "nighthaunt/GrimghastReapers.h"
#include "nighthaunt/BladegheistRevenants.h"
#include "nighthaunt/GlaivewraithStalkers.h"
#include "nighthaunt/DreadscytheHarridans.h"
#include "nighthaunt/SpiritHosts.h"
#include "nighthaunt/Hexwraiths.h"
#include "nighthaunt/Chainghasts.h"

#include "beastsofchaos/Gors.h"
#include "beastsofchaos/Ghorgon.h"
#include "beastsofchaos/Ungors.h"
#include "beastsofchaos/Bestigors.h"
#include "beastsofchaos/Bullgors.h"
#include "beastsofchaos/Cygor.h"
#include "beastsofchaos/Centigors.h"
#include "beastsofchaos/Doombull.h"

#include "slaanesh/Fiends.h"
#include "slaanesh/Seekers.h"
#include "slaanesh/Daemonettes.h"

#include "nurgle/LordOfPlagues.h"
#include "nurgle/Plaguebearers.h"
#include "nurgle/PutridBlightkings.h"
#include "nurgle/BeastsOfNurgle.h"
#include "nurgle/TheGlottkin.h"

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

#include "fec/CryptHorrors.h"
#include "fec/CryptGhouls.h"
#include "fec/CryptFlayers.h"
#include "fec/CryptInfernalCourtier.h"
#include "fec/AbhorrantArchregent.h"
#include "fec/AbhorrantGhoulKing.h"
#include "fec/GhoulKingOnTerrorgheist.h"
#include "fec/GhoulKingOnZombieDragon.h"
#include "fec/VarghulfCourtier.h"
#include "fec/CryptGhastCourtier.h"
#include "fec/CryptHaunterCourtier.h"
#include "fec/RoyalZombieDragon.h"
#include "fec/RoyalTerrorgheist.h"

#include "wanderers/GladeGuard.h"
#include "wanderers/EternalGuard.h"
#include "wanderers/WildwoodRangers.h"

#include "dispossessed/Hammerers.h"
#include "dispossessed/Ironbreakers.h"
#include "dispossessed/Irondrakes.h"
#include "dispossessed/Quarrellers.h"
#include "dispossessed/Longbeards.h"
#include "dispossessed/Warriors.h"
#include "dispossessed/Thunderers.h"

#include "skaven/Clanrats.h"
#include "skaven/GutterRunners.h"
#include "skaven/PlagueMonks.h"
#include "skaven/NightRunners.h"
#include "skaven/Stormfiends.h"
#include "skaven/Stormvermin.h"
#include "skaven/HellPitAbomination.h"
#include "skaven/RatlingGun.h"
#include "skaven/WarpfireThrower.h"
#include "skaven/WarpLightningCannon.h"

#include "tzeentch/KairicAcolytes.h"
#include "tzeentch/PinkHorrors.h"
#include "tzeentch/TzaangorEnlightened.h"
#include "tzeentch/Tzaangors.h"
#include "tzeentch/FlamersOfTzeentch.h"

#include "ironjawz/OrrukArdboys.h"
#include "ironjawz/OrrukBrutes.h"
#include "ironjawz/GoreGruntas.h"
#include "ironjawz/OrrukMegaboss.h"

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

    StormcastEternals::Liberators::Init();
    StormcastEternals::CelestarBallista::Init();
    StormcastEternals::Evocators::Init();
    StormcastEternals::Judicators::Init();
    StormcastEternals::Sequitors::Init();
    StormcastEternals::LordOrdinator::Init();
    StormcastEternals::Retributors::Init();
    StormcastEternals::LordCelestant::Init();
    StormcastEternals::LordCelestantOnDracoth::Init();
    StormcastEternals::LordCelestantOnStardrake::Init();
    StormcastEternals::KnightIncantor::Init();
    StormcastEternals::KnightQuestor::Init();
    StormcastEternals::KnightZephyros::Init();
    StormcastEternals::VanguardHunters::Init();
    StormcastEternals::Castigators::Init();
    StormcastEternals::VanguardRaptorsHurricane::Init();
    StormcastEternals::VanguardRaptorsLongstrike::Init();
    StormcastEternals::VandusHammerhand::Init();
    StormcastEternals::GryphHounds::Init();
    StormcastEternals::Concussors::Init();
    StormcastEternals::Desolators::Init();
    StormcastEternals::Fulminators::Init();
    StormcastEternals::Tempestors::Init();
    StormcastEternals::EvocatorsOnCelestialDracolines::Init();
    StormcastEternals::Prosecutors::Init();
    StormcastEternals::Protectors::Init();
    StormcastEternals::Decimators::Init();
    StormcastEternals::NeaveBlacktalon::Init();
    StormcastEternals::GavrielSureheart::Init();

    GloomspiteGitz::BoingrotBounderz::Init();
    GloomspiteGitz::ColossalSquig::Init();
    GloomspiteGitz::LoonbossOnManglerSquigs::Init();
    GloomspiteGitz::ManglerSquigs::Init();
    GloomspiteGitz::SquiqHerd::Init();
    GloomspiteGitz::SquiqHoppers::Init();
    GloomspiteGitz::Stabbas::Init();
    GloomspiteGitz::Shootas::Init();
    GloomspiteGitz::RockgutTroggoths::Init();
    GloomspiteGitz::DankholdTroggboss::Init();
    GloomspiteGitz::DankholdTroggoths::Init();
    GloomspiteGitz::Skragrott::Init();
    GloomspiteGitz::Loonboss::Init();
    GloomspiteGitz::LoonbossOnGiantCaveSquig::Init();
    GloomspiteGitz::LoonbossWithGiantCaveSquig::Init();
    GloomspiteGitz::AleguzzlerGargant::Init();
    GloomspiteGitz::SquigGobba::Init();
    GloomspiteGitz::SpiderRiders::Init();
    GloomspiteGitz::FellwaterTroggoths::Init();

    Khorne::Bloodreavers::Init();
    Khorne::BloodWarriors::Init();
    Khorne::Bloodletters::Init();
    Khorne::Wrathmongers::Init();
    Khorne::Skullreapers::Init();
    Khorne::Khorgoraths::Init();
    Khorne::BloodthirsterOfUnfetteredFury::Init();
    Khorne::BloodthirsterOfInsensateRage::Init();
    Khorne::WrathOfKhorneBloodthirster::Init();
    Khorne::Skarbrand::Init();
    Khorne::Karanak::Init();
    Khorne::FleshHounds::Init();
    Khorne::Skulltaker::Init();
    Khorne::Bloodmaster::Init();
    Khorne::Bloodcrushers::Init();
    Khorne::MightySkullcrushers::Init();
    Khorne::Skullmaster::Init();

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

    DaughtersOfKhaine::AvatarOfKhaine::Init();
    DaughtersOfKhaine::BloodSisters::Init();
    DaughtersOfKhaine::BloodStalkers::Init();
    DaughtersOfKhaine::BloodwrackMedusa::Init();
    DaughtersOfKhaine::DoomfireWarlocks::Init();
    DaughtersOfKhaine::HagQueen::Init();
    DaughtersOfKhaine::KhineraiHeartrenders::Init();
    DaughtersOfKhaine::KhineraiLifetakers::Init();
    DaughtersOfKhaine::SistersOfSlaughter::Init();
    DaughtersOfKhaine::SlaughterQueen::Init();
    DaughtersOfKhaine::WitchAelves::Init();

    IdonethDeepkin::NamartiThralls::Init();
    IdonethDeepkin::NamartiReavers::Init();
    IdonethDeepkin::AkhelianMorrsarrGuard::Init();
    IdonethDeepkin::AkhelianIshlaenGuard::Init();
    IdonethDeepkin::AkhelianLeviadon::Init();
    IdonethDeepkin::AkhelianKing::Init();
    IdonethDeepkin::AkhelianAllopexes::Init();

    Nighthaunt::ChainraspHorde::Init();
    Nighthaunt::GrimghastReapers::Init();
    Nighthaunt::BladegheistRevenants::Init();
    Nighthaunt::GlaivewraithStalkers::Init();
    Nighthaunt::DreadscytheHarridans::Init();
    Nighthaunt::Hexwraiths::Init();
    Nighthaunt::SpiritHosts::Init();
    Nighthaunt::Chainghasts::Init();

    BeastsOfChaos::Gors::Init();
    BeastsOfChaos::Ghorgon::Init();
    BeastsOfChaos::Bestigors::Init();
    BeastsOfChaos::Bullgors::Init();
    BeastsOfChaos::Ungors::Init();
    BeastsOfChaos::Cygor::Init();
    BeastsOfChaos::Centigors::Init();
    BeastsOfChaos::Doombull::Init();

    Slaanesh::Daemonettes::Init();
    Slaanesh::Fiends::Init();
    Slaanesh::Seekers::Init();

    Nurgle::LordOfPlagues::Init();
    Nurgle::Plaguebearers::Init();
    Nurgle::PutridBlightkings::Init();
    Nurgle::BeastsOfNurgle::Init();
    Nurgle::TheGlottkin::Init();

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

    FleshEaterCourt::CryptGhouls::Init();
    FleshEaterCourt::CryptHorrors::Init();
    FleshEaterCourt::CryptFlayers::Init();
    FleshEaterCourt::CryptInfernalCourtier::Init();
    FleshEaterCourt::AbhorrantArchregent::Init();
    FleshEaterCourt::AbhorrantGhoulKing::Init();
    FleshEaterCourt::AbhorrantGhoulKingOnTerrorgheist::Init();
    FleshEaterCourt::AbhorrantGhoulKingOnZombieDragon::Init();
    FleshEaterCourt::VarghulfCourtier::Init();
    FleshEaterCourt::RoyalTerrorgheist::Init();
    FleshEaterCourt::RoyalZombieDragon::Init();
    FleshEaterCourt::CryptGhastCourtier::Init();
    FleshEaterCourt::CryptHaunterCourtier::Init();

    Wanderers::GladeGuard::Init();
    Wanderers::EternalGuard::Init();
    Wanderers::WildwoodRangers::Init();

    Dispossessed::Hammerers::Init();
    Dispossessed::Ironbreakers::Init();
    Dispossessed::Irondrakes::Init();
    Dispossessed::Longbeards::Init();
    Dispossessed::Quarrellers::Init();
    Dispossessed::Thunderers::Init();
    Dispossessed::Warriors::Init();

    Skaven::Clanrats::Init();
    Skaven::GutterRunners::Init();
    Skaven::NightRunners::Init();
    Skaven::PlagueMonks::Init();
    Skaven::Stormfiends::Init();
    Skaven::Stormvermin::Init();
    Skaven::HellPitAbomination::Init();
    Skaven::RatlingGun::Init();
    Skaven::WarpfireThrower::Init();
    Skaven::WarpLightningCannon::Init();

    Tzeentch::KairicAcolytes::Init();
    Tzeentch::PinkHorrors::Init();
    Tzeentch::TzaangorEnlightened::Init();
    Tzeentch::Tzaangors::Init();
    Tzeentch::FlamersOfTzeentch::Init();

    Ironjawz::OrrukArdboys::Init();
    Ironjawz::OrrukBrutes::Init();
    Ironjawz::OrrukGoreGruntas::Init();
    Ironjawz::OrrukMegaboss::Init();

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
}
