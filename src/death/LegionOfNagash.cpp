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

namespace Death
{

std::string LegionOfNagashBase::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int LegionOfNagashBase::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Init()
{
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
}

} //namespace Seraphon
