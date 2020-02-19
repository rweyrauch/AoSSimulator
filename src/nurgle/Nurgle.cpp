/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Nurgle.h>
#include "nurgle/LordOfPlagues.h"
#include "nurgle/Plaguebearers.h"
#include "nurgle/PutridBlightkings.h"
#include "nurgle/BeastsOfNurgle.h"
#include "nurgle/TheGlottkin.h"
#include "nurgle/PlagueDrones.h"
#include "nurgle/PusgoyleBlightlords.h"
#include "nurgle/Rotigus.h"
#include "nurgle/GreatUncleanOne.h"
#include "nurgle/FeculaFlyblown.h"
#include "nurgle/TheWurmspat.h"

namespace Nurgle
{

std::string NurgleBase::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int NurgleBase::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Init()
{
    LordOfPlagues::Init();
    Plaguebearers::Init();
    PutridBlightkings::Init();
    BeastsOfNurgle::Init();
    TheGlottkin::Init();
    PlagueDrones::Init();
    PusgoyleBlightlords::Init();
    Rotigus::Init();
    GreatUncleanOne::Init();
    FeculaFlyblown::Init();
    TheWurmspat::Init();
}

} //namespace Nurgle
