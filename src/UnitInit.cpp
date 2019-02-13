/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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

#include "sylvaneth/Alarielle.h"
#include "sylvaneth/KurnothHunters.h"
#include "sylvaneth/SpiritOfDurthu.h"
#include "sylvaneth/Dryads.h"
#include "sylvaneth/TreeRevenants.h"
#include "sylvaneth/SpiteRevenants.h"

#include "dok/WitchAelves.h"
#include "dok/BloodSisters.h"
#include "dok/KhineraiLifetakers.h"
#include "dok/SistersOfSlaughter.h"

#include "idonethdeepkin/NamartiThralls.h"
#include "idonethdeepkin/NamartiReavers.h"
#include "idonethdeepkin/IshlaenGuard.h"
#include "idonethdeepkin/MorrsarrGuard.h"

#include "nighthaunt/ChainraspHorde.h"
#include "nighthaunt/GrimghastReapers.h"
#include "nighthaunt/BladegheistRevenants.h"
#include "nighthaunt/GlaivewraithStalkers.h"
#include "nighthaunt/DreadscytheHarridans.h"

#include "beastsofchaos/Gors.h"
#include "beastsofchaos/Ghorgon.h"
#include "beastsofchaos/Ungors.h"
#include "beastsofchaos/Bestigors.h"
#include "beastsofchaos/Bullgors.h"

#include "slaanesh/Fiends.h"
#include "slaanesh/Seekers.h"
#include "slaanesh/Daemonettes.h"

#include "nurgle/LordOfPlagues.h"
#include "nurgle/Plaguebearers.h"
#include "nurgle/PutridBlightkings.h"

#include "death/BlackKnights.h"
#include "death/BloodKnights.h"
#include "death/DireWolves.h"
#include "death/GraveGuard.h"
#include "death/SkeletonWarriors.h"
#include "death/Vargheists.h"
#include "death/Zombies.h"

#include "fec/CryptHorrors.h"
#include "fec/CryptGhouls.h"
#include "fec/CryptFlayers.h"
#include "fec/CryptInfernalCourtier.h"

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

static Verbosity g_verbosity = Verbosity::Normal;

Verbosity GetVerbosity()
{
    return g_verbosity;
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

    Sylvaneth::Alarielle::Init();
    Sylvaneth::KurnothHunters::Init();
    Sylvaneth::SpiritOfDurthu::Init();
    Sylvaneth::Dryads::Init();
    Sylvaneth::TreeRevenants::Init();
    Sylvaneth::SpiteRevenants::Init();

    DaughtersOfKhaine::WitchAelves::Init();
    DaughtersOfKhaine::SistersOfSlaughter::Init();
    DaughtersOfKhaine::BloodSisters::Init();
    DaughtersOfKhaine::KhineraiLifetakers::Init();

    IdonethDeepkin::NamartiThralls::Init();
    IdonethDeepkin::NamartiReavers::Init();
    IdonethDeepkin::AkhelianMorrsarrGuard::Init();
    IdonethDeepkin::AkhelianIshlaenGuard::Init();

    Nighthaunt::ChainraspHorde::Init();
    Nighthaunt::GrimghastReapers::Init();
    Nighthaunt::BladegheistRevenants::Init();
    Nighthaunt::GlaivewraithStalkers::Init();
    Nighthaunt::DreadscytheHarridans::Init();

    BeastsOfChaos::Gors::Init();
    BeastsOfChaos::Ghorgon::Init();
    BeastsOfChaos::Bestigors::Init();
    BeastsOfChaos::Bullgors::Init();
    BeastsOfChaos::Ungors::Init();

    Slaanesh::Daemonettes::Init();
    Slaanesh::Fiends::Init();
    Slaanesh::Seekers::Init();

    Nurgle::LordOfPlagues::Init();
    Nurgle::Plaguebearers::Init();
    Nurgle::PutridBlightkings::Init();

    Death::BlackKnights::Init();
    Death::BloodKnights::Init();
    Death::DireWolves::Init();
    Death::GraveGuard::Init();
    Death::SkeletonWarriors::Init();
    Death::Vargheists::Init();
    Death::Zombies::Init();

    FleshEaterCourt::CryptGhouls::Init();
    FleshEaterCourt::CryptHorrors::Init();
    FleshEaterCourt::CryptFlayers::Init();
    FleshEaterCourt::CryptInfernalCourtier::Init();

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
}