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
    if (std::string(parameter.name) == "Way of the Seraphon")
    {
        if (parameter.intValue == Starborne) return "Starborne";
        else if (parameter.intValue == Coalesced) return "Coalesced";
    }
    else if (std::string(parameter.name) == "Constellation")
    {
        if (parameter.intValue == KoatlsClaw) return "Koatl's Claw";
        else if (parameter.intValue == ThunderLizard) return "ThunderLizard";
        else if (parameter.intValue == DracothionsTail) return "Dracothion's Tail";
        else if (parameter.intValue == FangsOfSotek) return "Fangs of Sotek";
        else if (parameter.intValue == None) return "None";
    }
    return ParameterValueToString(parameter);
}

int SeraphonBase::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Starborne") return Starborne;
    else if (enumString == "Coalesced") return Coalesced;
    else if (enumString == "Koatl's Claw") return KoatlsClaw;
    else if (enumString == "ThunderLizard") return ThunderLizard;
    else if (enumString == "Dracothion's Tail") return DracothionsTail;
    else if (enumString == "Fangs of Sotek") return FangsOfSotek;
    else if (enumString == "None") return None;

    return 0;
}

void SeraphonBase::setWayOfTheSeraphon(SeraphonBase::WayOfTheSeraphon way, Constellation constellation)
{
    // TODO: validate the combination of way and constellation and warn caller
    if (way == Starborne)
    {
        if (constellation == KoatlsClaw || constellation == ThunderLizard)
            constellation = None;
    }
    else if (way == Coalesced)
    {
        if (constellation == DracothionsTail || constellation == FangsOfSotek)
            constellation = None;
    }

    removeKeyword(COALESCED);
    removeKeyword(STARBORNE);
    removeKeyword(KOATLS_CLAW);
    removeKeyword(THUNDER_LIZARD);
    removeKeyword(DRACOTHIONS_TAIL);
    removeKeyword(FANGS_OF_SOTEK);

    if (way == Coalesced) addKeyword(COALESCED);
    else if (way == Starborne) addKeyword(STARBORNE);

    if (constellation == KoatlsClaw) addKeyword(KOATLS_CLAW);
    else if (constellation == ThunderLizard) addKeyword(THUNDER_LIZARD);
    else if (constellation == DracothionsTail) addKeyword(DRACOTHIONS_TAIL);
    else if (constellation == FangsOfSotek) addKeyword(FANGS_OF_SOTEK);

    m_way = way;
    m_constellation = constellation;
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
