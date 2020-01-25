/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slavestodarkness/SlavesToDarkness.h>

#include "slavestodarkness/ChaosChosen.h"
#include "slavestodarkness/ChaosKnights.h"
#include "slavestodarkness/ChaosMarauders.h"
#include "slavestodarkness/ChaosWarriors.h"
#include "slavestodarkness/CorvusCabal.h"
#include "slavestodarkness/CypherLords.h"
#include "slavestodarkness/Furies.h"
#include "slavestodarkness/IronGolems.h"
#include "slavestodarkness/Raptoryx.h"
#include "slavestodarkness/Slambo.h"
#include "slavestodarkness/SplinteredFang.h"
#include "slavestodarkness/TheUnmade.h"
#include "slavestodarkness/UntamedBeasts.h"
#include "slavestodarkness/ChaosLordOnKarkadrak.h"
#include "slavestodarkness/ChaosLordOnManticore.h"
#include "slavestodarkness/ChaosWarshrine.h"
#include "slavestodarkness/ChaosSorcererOnManticore.h"
#include "slavestodarkness/ChaosSorcerer.h"
#include "slavestodarkness/DarkoathChieftain.h"
#include "slavestodarkness/DarkoathWarqueen.h"
#include "slavestodarkness/ExaltedHeroOfChaos.h"
#include "slavestodarkness/ChaosLordOnDaemonicMount.h"
#include "slavestodarkness/ChaosLord.h"
#include "slavestodarkness/ChaosChariots.h"
#include "slavestodarkness/GorebeastChariots.h"

namespace SlavesToDarkness
{

std::string SlavesToDarknessBase::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Damned Legion")
    {
        if (parameter.intValue == Ravagers) return "Ravagers";
        else if (parameter.intValue == Cabalists) return "Cabalists";
        else if (parameter.intValue == Despoilers) return "Despoilers";
        else if (parameter.intValue == HostOfTheEverchosen) return "Host of the Everchosen";
    }
    return ParameterValueToString(parameter);
}

int SlavesToDarknessBase::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Ravagers") return Ravagers;
    else if (enumString == "Cabalists") return Cabalists;
    else if (enumString == "Despoilers") return Despoilers;
    else if (enumString == "Host of the Everchosen") return HostOfTheEverchosen;

    return 0;
}

void SlavesToDarknessBase::setDamnedLegion(DamnedLegion legion)
{
    removeKeyword(RAVAGERS);
    removeKeyword(CABALISTS);
    removeKeyword(DESPOILERS);
    removeKeyword(HOST_OF_THE_EVERCHOSEN);

    m_legion = legion;
    switch (legion)
    {
        case Ravagers:
            addKeyword(RAVAGERS);
            break;
        case Cabalists:
            addKeyword(CABALISTS);
            break;
        case Despoilers:
            addKeyword(DESPOILERS);
            break;
        case HostOfTheEverchosen:
            addKeyword(HOST_OF_THE_EVERCHOSEN);
            break;
        default:
            break;
    }
}

void Init()
{
    ChaosChosen::Init();
    ChaosKnights::Init();
    ChaosMarauders::Init();
    ChaosWarriors::Init();
    CorvusCabal::Init();
    CypherLords::Init();
    Furies::Init();
    IronGolems::Init();
    Raptoryx::Init();
    Slambo::Init();
    SplinteredFang::Init();
    TheUnmade::Init();
    UntamedBeasts::Init();
    ChaosLordOnKarkadrak::Init();
    ChaosLordOnManticore::Init();
    ChaosWarshrine::Init();
    ChaosSorcererOnManticore::Init();
    ChaosSorcerer::Init();
    DarkoathChieftain::Init();
    DarkoathWarqueen::Init();
    ExaltedHeroOfChaos::Init();
    ChaosLordOnDaemonicMount::Init();
    ChaosLord::Init();
    ChaosChariots::Init();
    GorebeastChariots::Init();
}

} //namespace SlavesToDarkness
