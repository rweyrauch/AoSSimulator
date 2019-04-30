/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/BeastsOfChaosBase.h>
#include "beastsofchaos/Gors.h"
#include "beastsofchaos/Ghorgon.h"
#include "beastsofchaos/Ungors.h"
#include "beastsofchaos/Bestigors.h"
#include "beastsofchaos/Bullgors.h"
#include "beastsofchaos/Cygor.h"
#include "beastsofchaos/Centigors.h"
#include "beastsofchaos/Doombull.h"
#include "beastsofchaos/Chimera.h"
#include "beastsofchaos/Beastlord.h"
#include "beastsofchaos/GreatBrayShaman.h"
#include "beastsofchaos/UngorRaiders.h"
#include "beastsofchaos/TuskgorChariots.h"

namespace BeastsOfChaos
{

void BeastsOfChaosBase::setGreatfray(BeastsOfChaosBase::Greatfray fray)
{

}

std::string BeastsOfChaosBase::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Greatfray")
    {
        if (parameter.m_intValue == Allherd) { return "Allherd"; }
        else if (parameter.m_intValue == Darkwalkers) { return "Darkwalkers"; }
        else if (parameter.m_intValue == Gavespawn) { return "Gavespawn"; }
        else if (parameter.m_intValue == None) { return "None"; }
    }
    return ParameterValueToString(parameter);
}

int BeastsOfChaosBase::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Allherd") { return Allherd; }
    else if (enumString == "Darkwalkers") { return Darkwalkers; }
    else if (enumString == "Gavespawn") { return Gavespawn; }
    else if (enumString == "None") { return None; }
    return 0;
}

void Init()
{
    Gors::Init();
    Ghorgon::Init();
    Bestigors::Init();
    Bullgors::Init();
    Ungors::Init();
    Cygor::Init();
    Centigors::Init();
    Doombull::Init();
    Chimera::Init();
    Beastlord::Init();
    GreatBrayShaman::Init();
    UngorRaiders::Init();
    TuskgorChariots::Init();
}

} // namespace BeastsOfChaos
