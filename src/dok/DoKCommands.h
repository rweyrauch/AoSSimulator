/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "CommandAbility.h"
#include "CommandTraitAbility.h"
#include <dok/DaughterOfKhaine.h>

namespace DaughtersOfKhaine {

    CommandAbility *CreateSendForthTheCauldrons(Unit *hero);

    CommandAbility *CreateAThousandBladeforms(Unit *hero);

    CommandAbility *CreateInspiredByCarnage(Unit *hero);

    CommandAbility *CreateMastersOfTheShadowpaths(Unit *hero);

    CommandAbility *CreateBleedTheMind(Unit *hero);

    CommandAbility *CreatePowerInTheBlood(Unit *hero);

    CommandAbility *CreateCommandAbility(Temple temple, Unit *hero);

    CommandTraitAbility *CreateCommandTrait(CommandTrait which, Unit *general);

    CommandTraitAbility *CreateCommandTrait(Temple temple, Unit *general);

}
