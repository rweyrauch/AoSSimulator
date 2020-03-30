/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Skaventide.h>
#include <UnitFactory.h>

#include "skaven/ArchWarlock.h"
#include "skaven/Clanrats.h"
#include "skaven/Clawlord.h"
#include "skaven/Deathmaster.h"
#include "skaven/Doomflayer.h"
#include "skaven/Doomwheel.h"
#include "skaven/GiantRats.h"
#include "skaven/GreySeer.h"
#include "skaven/GreySeerScreamingBell.h"
#include "skaven/GutterRunners.h"
#include "skaven/HellPitAbomination.h"
#include "skaven/LordSkreech.h"
#include "skaven/MasterMoulder.h"
#include "skaven/NightRunners.h"
#include "skaven/Packmasters.h"
#include "skaven/PlagueCenserBearers.h"
#include "skaven/Plagueclaw.h"
#include "skaven/PlagueMonks.h"
#include "skaven/PlaguePriestFurnace.h"
#include "skaven/PlaguePriest.h"
#include "skaven/RatlingGun.h"
#include "skaven/RatOgors.h"
#include "skaven/RatSwarms.h"
#include "skaven/SkryeAcolytes.h"
#include "skaven/Stormfiends.h"
#include "skaven/Stormvermin.h"
#include "skaven/ThanquolBoneripper.h"
#include "skaven/VerminlordCorruptor.h"
#include "skaven/VerminlordDeceiver.h"
#include "skaven/VerminlordWarbringer.h"
#include "skaven/VerminlordWarpseer.h"
#include "skaven/WarlockBombardier.h"
#include "skaven/WarlockEngineer.h"
#include "skaven/WarpfireThrower.h"
#include "skaven/WarpGrinder.h"
#include "skaven/WarpLightningCannon.h"
#include "skaven/WarplockJezzails.h"
#include "skaven/SkritchSpiteclaw.h"
#include "skaven/SpiteclawsSwarm.h"

namespace Skaven {

    int Skaventide::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toHitModifier(weapon, target);

        // Overwhelming Mass
        if (!weapon->isMissile() && remainingModels() >= 20)
            modifier += 1;

        return modifier;
    }

    int Skaventide::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toWoundModifier(weapon, target);

        // Overwhelming Mass
        if (!weapon->isMissile() && remainingModels() >= 30)
            modifier += 1;

        return modifier;
    }

    int Skaventide::braveryModifier() const {
        int modifier = Unit::braveryModifier();

        // Strength in Numbers (add an additional +1 for each 10 models)
        modifier += remainingModels() / 10;

        return modifier;
    }

    bool Skaventide::moreMore() const {
        return (Dice::rollD6() >= 3);
    }

    std::string Skaventide::ValueToString(const Parameter &parameter) {
        return "";
    }

    int Skaventide::EnumStringToInt(const std::string &enumString) {
        return 0;
    }

    void Init() {
        ArchWarlock::Init();
        Clanrats::Init();
        Clawlord::Init();
        Deathmaster::Init();
        Doomflayer::Init();
        Doomwheel::Init();
        GiantRats::Init();
        GreySeer::Init();
        GreySeerOnScreamingBell::Init();
        GutterRunners::Init();
        HellPitAbomination::Init();
        LordSkreechVerminking::Init();
        MasterMoulder::Init();
        NightRunners::Init();
        Packmasters::Init();
        PlagueCenserBearers::Init();
        Plagueclaw::Init();
        PlagueMonks::Init();
        PlaguePriestOnPlagueFurnace::Init();
        PlaguePriest::Init();
        RatlingGun::Init();
        RatOgors::Init();
        RatSwarms::Init();
        SkritchSpiteclaw::Init();
        SkryeAcolytes::Init();
        SpiteclawsSwarm::Init();
        Stormfiends::Init();
        Stormvermin::Init();
        ThanquolOnBoneripper::Init();
        VerminlordCorruptor::Init();
        VerminlordDeceiver::Init();
        VerminlordWarbringer::Init();
        VerminlordWarpseer::Init();
        WarlockBombardier::Init();
        WarlockEngineer::Init();
        WarpfireThrower::Init();
        WarpGrinder::Init();
        WarpLightningCannon::Init();
        WarplockJezzails::Init();
    }

} // namespace Skaven