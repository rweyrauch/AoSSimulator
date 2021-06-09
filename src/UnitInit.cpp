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
#include "tzeentch/TzeentchBase.h"
#include "lumineth/LuminethRealmLords.h"
#include "behemat/SonsOfBehemat.h"
#include "soulblight/SoulblightGravelords.h"
#include "kruleboyz/KruleBoyz.h"

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

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <freeagents/Kragnos.h>

void Initialize(plog::Severity level) {

    static plog::ColorConsoleAppender<plog::MessageOnlyFormatter> consoleAppender;
    plog::init(level, "AosSimLog.txt").addAppender(&consoleAppender);

    StormcastEternals::Init();
    GloomspiteGitz::Init();
    FleshEaterCourt::Init();
    DaughtersOfKhaine::Init();
    Nighthaunt::Init();
    Skaven::Init();
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
    LuminethRealmLords::Init();
    SonsOfBehemat::Init();
    Soulblight::Init();
    Kruleboyz::Init();

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
    Destruction::Kragnos::Init();
}
