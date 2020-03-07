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
#include "seraphon/Bastiladon.h"
#include "seraphon/Salamanders.h"
#include "seraphon/Razordons.h"
#include "seraphon/Kroxigor.h"
#include "seraphon/RipperdactylRiders.h"
#include "seraphon/TerradonRiders.h"
#include "seraphon/Stegadon.h"
#include "seraphon/EngineOfTheGods.h"
#include "seraphon/SaurusVeteranColdOne.h"
#include "seraphon/ChameleonSkinks.h"
#include "seraphon/RipperdactylChief.h"
#include "seraphon/TerradonChief.h"
#include "seraphon/SaurusEternityWarden.h"
#include "seraphon/SaurusAstrolithBearer.h"
#include "seraphon/LordKroak.h"
#include "seraphon/SkinkStarseer.h"
#include "seraphon/SkinkStarpriest.h"
#include "seraphon/SkinkPriest.h"
#include "seraphon/SlannStarmaster.h"

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
    Salamanders::Init();
    Razordons::Init();
    Troglodon::Init();
    Bastiladon::Init();
    Kroxigor::Init();
    RipperdactylRiders::Init();
    TerradonRiders::Init();
    Stegadon::Init();
    EngineOfTheGods::Init();
    SaurusScarVeteranOnColdOne::Init();
    ChameleonSkinks::Init();
    TerradonChief::Init();
    RipperdactylChief::Init();
    SaurusAstrolithBearer::Init();
    SaurusEternityWarden::Init();
    LordKroak::Init();
    SkinkStarseer::Init();
    SkinkPriest::Init();
    SkinkStarpriest::Init();
    SlannStarmaster::Init();
}

} //namespace Seraphon
