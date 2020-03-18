/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "nurgle/BeastsOfNurgle.h"
#include "nurgle/BloabRotspawned.h"
#include "nurgle/Epidemius.h"
#include "nurgle/FeculaFlyblown.h"
#include "nurgle/Festus.h"
#include "nurgle/GreatUncleanOne.h"
#include "nurgle/Gutrot.h"
#include "nurgle/HarbingerOfDecay.h"
#include "nurgle/HorticulousSlimux.h"
#include "nurgle/LordOfAfflictions.h"
#include "nurgle/LordOfBlights.h"
#include "nurgle/LordOfPlagues.h"
#include "nurgle/MorbidexTwiceborn.h"
#include "nurgle/OrghottsDaemonspew.h"
#include "nurgle/Plaguebearers.h"
#include "nurgle/PlagueDrones.h"
#include "nurgle/Poxbringer.h"
#include "nurgle/PusgoyleBlightlords.h"
#include "nurgle/PutridBlightkings.h"
#include "nurgle/Rotigus.h"
#include "nurgle/SloppityBilepiper.h"
#include "nurgle/SpoilpoxScrivener.h"
#include "nurgle/TheGlottkin.h"
#include "nurgle/TheWurmspat.h"
#include "nurgle/Nurglings.h"

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
    BloabRotspawned::Init();
    EpidemiusTallymanOfNurgle::Init();
    FestusTheLeechlord::Init();
    GutrotSpume::Init();
    HarbingerOfDecay::Init();
    HorticulousSlimux::Init();
    LordOfAfflictions::Init();
    LordOfBlights::Init();
    OrghottsDaemonspew::Init();
    PoxbringerHeraldOfNurgle::Init();
    SloppityBilepiperHeraldOfNurgle::Init();
    SpoilpoxScrivenerHeraldOfNurgle::Init();
    Nurglings::Init();
}

} //namespace Nurgle
