/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/LegionOfNagash.h>

#include "death/Nagash.h"
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
#include "death/BatSwarms.h"
#include "death/BloodseekerPalangquin.h"
#include "death/CairnWraith.h"
#include "death/CorpseCartBrazier.h"
#include "death/CorpseCartLodestone.h"
#include "death/CovenThrone.h"
#include "death/FellBats.h"
#include "death/Mannfred.h"
#include "death/MortisEngine.h"
#include "death/Neferata.h"
#include "death/PrinceVhordrai.h"
#include "death/VampireLord.h"
#include "death/VampireLordOnDragon.h"

namespace Death {

    std::string LegionOfNagashBase::ValueToString(const Parameter &parameter) {
        return ParameterValueToString(parameter);
    }

    int LegionOfNagashBase::EnumStringToInt(const std::string &enumString) {
        return 0;
    }

    void Init() {
        Nagash::Init();
        BlackKnights::Init();
        BloodKnights::Init();
        DireWolves::Init();
        GraveGuard::Init();
        SkeletonWarriors::Init();
        Vargheists::Init();
        Zombies::Init();
        WightKingWithBlackAxe::Init();
        WightKingWithBalefulTombBlade::Init();
        Necromancer::Init();
        BatSwarms::Init();
        BloodseekerPalanquin::Init();
        CairnWraith::Init();
        CorpseCartWithBalefireBrazier::Init();
        CorpseCartWithUnholyLodestone::Init();
        CovenThrone::Init();
        FellBats::Init();
        MannfredMortarchOfNight::Init();
        MortisEngine::Init();
        NeferataMortarchOfBlood::Init();
        PrinceVhordrai::Init();
        VampireLord::Init();
        VampireLordOnZombieDragon::Init();
    }

} //namespace Seraphon
