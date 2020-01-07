/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/KharadronBase.h>
#include <kharadron/AethericNavigator.h>
#include <kharadron/AetherKhemist.h>
#include <kharadron/ArkanautAdmiral.h>
#include <kharadron/ArkanautCompany.h>
#include <kharadron/ArkanautFrigate.h>
#include <kharadron/ArkanautIronclad.h>
#include <kharadron/BrokkGrungsson.h>
#include <kharadron/EndrinmasterWithHarness.h>
#include <kharadron/EndrinmasterWithSuit.h>
#include <kharadron/Endrinriggers.h>
#include <kharadron/GrundstokGunhauler.h>
#include <kharadron/GrundstokThunderers.h>
#include <kharadron/Skywardens.h>

namespace KharadronOverlords
{

std::string KharadronBase::ValueToString(const Parameter& parameter)
{
    if (parameter.m_name == "Skyport")
    {
        if (parameter.m_intValue == BarakNar) return "Barak-Nar";
        else if (parameter.m_intValue == BarakZon) return "Barak-Zon";
        else if (parameter.m_intValue == BarakZilfin) return "Barak-Zilfin";
        else if (parameter.m_intValue == BarakUrbaz) return "Barak-Urbaz";
        else if (parameter.m_intValue == BarakThryng) return "Barak-Thryng";
        else if (parameter.m_intValue == BarakMhornar) return "Barak-Mhornar";
        else if (parameter.m_intValue == None) return "None";
        else if (parameter.m_intValue == Custom) return "Custom";
    }
    return ParameterValueToString(parameter);
}

int KharadronBase::EnumStringToInt(const std::string& enumString)
{
    if (enumString == "Barak=Nar") return BarakNar;
    else if (enumString == "Barak-Zon") return BarakZon;
    else if (enumString == "BarakZilfin") return BarakZilfin;
    else if (enumString == "BarakUrbaz") return BarakUrbaz;
    else if (enumString == "BarakThryng") return BarakThryng;
    else if (enumString == "BarakMhornar") return BarakMhornar;
    else if (enumString == "None") return None;
    else if (enumString == "Custom") return Custom;
    return 0;
}

void KharadronBase::setSkyport(KharadronBase::Skyport skyport)
{
    m_skyport = skyport;
    switch (skyport)
    {
        case BarakNar:
            addKeyword(BARAK_NAR);
            break;
        case BarakZon:
            addKeyword(BARAK_ZON);
            break;
        case BarakZilfin:
            addKeyword(BARAK_ZILFIN);
            break;
        case BarakUrbaz:
            addKeyword(BARAK_URBAZ);
            break;
        case BarakThryng:
            addKeyword(BARAK_THRYNG);
            break;
        case BarakMhornar:
            addKeyword(BARAK_MHORNAR);
            break;
        default:
            break;
    }
}

void Init()
{
    AethericNavigator::Init();
    AetherKhemist::Init();
    ArkanautAdmiral::Init();
    ArkanautCompany::Init();
    ArkanautFrigate::Init();
    ArkanautIronclad::Init();
    BrokkGrungsson::Init();
    EndrinmasterWithDirigibleSuit::Init();
    EndrinmasterWithEndrinharness::Init();
    Endrinriggers::Init();
    GrundstokGunhauler::Init();
    GrundstokThunderers::Init();
    Skywardens::Init();
}

} //KharadronOverlords
