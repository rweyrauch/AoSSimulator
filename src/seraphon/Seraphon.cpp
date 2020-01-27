/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Seraphon.h>
#include "seraphon/DreadSaurian.h"
#include "seraphon/SaurusGuard.h"
#include "seraphon/SaurusKnights.h"
#include "seraphon/SaurusWarriors.h"
#include "seraphon/Skinks.h"
#include "seraphon/SaurusOldblood.h"
#include "seraphon/SaurusSunblood.h"
#include "seraphon/SaurusOldbloodCarnosaur.h"
#include "seraphon/SaurusVeteranCarnosaur.h"
#include "seraphon/Troglodon.h"

namespace Seraphon
{

std::string SeraphonBase::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int SeraphonBase::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Init()
{
    DreadSaurian::Init();
    SaurusGuard::Init();
    SaurusKnights::Init();
    SaurusWarriors::Init();
    Skinks::Init();
    SaurusOldblood::Init();
    SaurusSunblood::Init();
    SaurusOldbloodOnCarnosaur::Init();
    SaurusScarVeteranOnCarnosaur::Init();
    Troglodon::Init();
}

} //namespace Seraphon
