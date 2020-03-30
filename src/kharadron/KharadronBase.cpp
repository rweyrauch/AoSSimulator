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
#include "kharadron/BjorgenThundrik.h"
#include "kharadron/ThundriksProfiteers.h"

namespace KharadronOverlords {

    std::string KharadronBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Skyport") {
            if (parameter.intValue == BarakNar) return "Barak-Nar";
            else if (parameter.intValue == BarakZon) return "Barak-Zon";
            else if (parameter.intValue == BarakZilfin) return "Barak-Zilfin";
            else if (parameter.intValue == BarakUrbaz) return "Barak-Urbaz";
            else if (parameter.intValue == BarakThryng) return "Barak-Thryng";
            else if (parameter.intValue == BarakMhornar) return "Barak-Mhornar";
            else if (parameter.intValue == None) return "None";
            else if (parameter.intValue == Custom) return "Custom";
        } else if (std::string(parameter.name) == "Artycle") {
            if (parameter.intValue == HonourIsEverything) return "Honour is Everything";
            else if (parameter.intValue == MasterTheSkies) return "Master the Skies";
            else if (parameter.intValue == SettleTheGrudges) return "Settle the Grudges";
        } else if (std::string(parameter.name) == "Amendment") {
            if (parameter.intValue == AlwaysTakeWhatYouAreOwed) return "Always Take What You Are Owed";
            else if (parameter.intValue == ProsecuteWarsWithAllHaste) return "Prosecute Wars With All Haste";
            else if (parameter.intValue == TrustToYourGuns) return "Trust To Your Guns";
        } else if (std::string(parameter.name) == "Footnote") {
            if (parameter.intValue == TheresNoRewardWithoutRisk) return "There's No Reward Without Risk";
            else if (parameter.intValue == TheresNoTradingWithSomePeople) return "There's No Trading With Some People";
            else if (parameter.intValue == WithoutOurShipsWeAreNaught) return "Without Our Ships, We Are Naught";
        }
        return ParameterValueToString(parameter);
    }

    int KharadronBase::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Barak=Nar") return BarakNar;
        else if (enumString == "Barak-Zon") return BarakZon;
        else if (enumString == "BarakZilfin") return BarakZilfin;
        else if (enumString == "BarakUrbaz") return BarakUrbaz;
        else if (enumString == "BarakThryng") return BarakThryng;
        else if (enumString == "BarakMhornar") return BarakMhornar;
        else if (enumString == "None") return None;
        else if (enumString == "Custom") return Custom;
        else if (enumString == "Honour is Everything") return HonourIsEverything;
        else if (enumString == "Master the Skies") return MasterTheSkies;
        else if (enumString == "Settle the Grudges") return SettleTheGrudges;
        else if (enumString == "Always Take What You Are Owed") return AlwaysTakeWhatYouAreOwed;
        else if (enumString == "Prosecute Wars With All Haste") return ProsecuteWarsWithAllHaste;
        else if (enumString == "Trust To Your Guns") return TrustToYourGuns;
        else if (enumString == "There's No Reward Without Risk") return TheresNoRewardWithoutRisk;
        else if (enumString == "There's No Trading With Some People") return TheresNoTradingWithSomePeople;
        else if (enumString == "Without Our Ships, We Are Naught") return WithoutOurShipsWeAreNaught;
        return 0;
    }

    void KharadronBase::setSkyport(KharadronBase::Skyport skyport) {
        removeKeyword(BARAK_NAR);
        removeKeyword(BARAK_ZON);
        removeKeyword(BARAK_ZILFIN);
        removeKeyword(BARAK_URBAZ);
        removeKeyword(BARAK_THRYNG);
        removeKeyword(BARAK_MHORNAR);

        m_skyport = skyport;
        switch (skyport) {
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

    void KharadronBase::setCode(KharadronBase::Artycle artycle, KharadronBase::Amendment amendment,
                                KharadronBase::Footnote footnote) {
        if (m_skyport != Custom) {
            // Error.
        }
        m_artycle = artycle;
        m_amendment = amendment;
        m_footnote = footnote;
    }

    void Init() {
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
        BjorgenThundrik::Init();
        ThundriksProfiteers::Init();
    }

} //KharadronOverlords
