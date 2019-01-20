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

#include "gloomspitegitz/BoingrotBounderz.h"
#include "gloomspitegitz/ColossalSquig.h"
#include "gloomspitegitz/LoonbossOnManglerSquigs.h"
#include "gloomspitegitz/ManglerSquigs.h"
#include "gloomspitegitz/SquigHerd.h"
#include "gloomspitegitz/SquigHoppers.h"
#include "gloomspitegitz/Stabbas.h"
#include "gloomspitegitz/Shootas.h"

#include "khorne/Bloodreavers.h"
#include "khorne/BloodWarriors.h"
#include "khorne/Bloodletters.h"

#include "sylvaneth/Alarielle.h"
#include "sylvaneth/KurnothHunters.h"
#include "sylvaneth/SpiritOfDurthu.h"
#include "sylvaneth/Dryads.h"
#include "sylvaneth/TreeRevenants.h"
#include "sylvaneth/SpiteRevenants.h"

#include "dok/WitchAelves.h"

#include "idonethdeepkin/NamartiThralls.h"

#include "nighthaunt/ChainraspHorde.h"
#include "nighthaunt/GrimghastReapers.h"
#include "nighthaunt/BladegheistRevenants.h"
#include "nighthaunt/GlaivewraithStalkers.h"
#include "nighthaunt/DreadscytheHarridans.h"

void Initialize()
{
    StormcastEternals::Liberators::Init();
    StormcastEternals::CelestarBallista::Init();
    StormcastEternals::Evocators::Init();
    StormcastEternals::Judicators::Init();
    StormcastEternals::Sequitors::Init();
    StormcastEternals::LordOrdinator::Init();
    StormcastEternals::Retributors::Init();

    GloomspiteGitz::BoingrotBounderz::Init();
    GloomspiteGitz::ColossalSquig::Init();
    GloomspiteGitz::LoonbossOnManglerSquigs::Init();
    GloomspiteGitz::ManglerSquigs::Init();
    GloomspiteGitz::SquiqHerd::Init();
    GloomspiteGitz::SquiqHoppers::Init();
    GloomspiteGitz::Stabbas::Init();
    GloomspiteGitz::Shootas::Init();

    Khorne::Bloodreavers::Init();
    Khorne::BloodWarriors::Init();
    Khorne::Bloodletters::Init();

    Sylvaneth::Alarielle::Init();
    Sylvaneth::KurnothHunters::Init();
    Sylvaneth::SpiritOfDurthu::Init();
    Sylvaneth::Dryads::Init();
    Sylvaneth::TreeRevenants::Init();
    Sylvaneth::SpiteRevenants::Init();

    DaughtersOfKhaine::WitchAelves::Init();

    IdonethDeepkin::NamartiThralls::Init();

    Nighthaunt::ChainraspHorde::Init();
    Nighthaunt::GrimghastReapers::Init();
    Nighthaunt::BladegheistRevenants::Init();
    Nighthaunt::GlaivewraithStalkers::Init();
    Nighthaunt::DreadscytheHarridans::Init();
}