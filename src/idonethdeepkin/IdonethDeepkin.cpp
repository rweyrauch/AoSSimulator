/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/IdonethDeepkin.h>
#include "idonethdeepkin/NamartiThralls.h"
#include "idonethdeepkin/NamartiReavers.h"
#include "idonethdeepkin/IshlaenGuard.h"
#include "idonethdeepkin/MorrsarrGuard.h"
#include "idonethdeepkin/AkhelianLeviadon.h"
#include "idonethdeepkin/AkhelianKing.h"
#include "idonethdeepkin/AkhelianAllopexes.h"
#include "idonethdeepkin/EidolonSea.h"
#include "idonethdeepkin/EidolonStorm.h"
#include "idonethdeepkin/Volturnos.h"
#include "idonethdeepkin/Lotann.h"
#include "idonethdeepkin/IsharannSoulscryer.h"
#include "idonethdeepkin/IsharannSoulrender.h"
#include "idonethdeepkin/IsharannTidecaster.h"

namespace IdonethDeepkin {

    std::string IdonethDeepkinBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Enclave") {
            if (parameter.intValue == None) return "None";
            else if (parameter.intValue == Ionrach) return "Ionrach";
            else if (parameter.intValue == DhomHain) return "Dhom-hain";
            else if (parameter.intValue == Fuethan) return "Fuethan";
            else if (parameter.intValue == Morphann) return "Mor'phann";
            else if (parameter.intValue == Nautilar) return "Nautilar";
            else if (parameter.intValue == Briomdar) return "Briomdar";
        }
        return ParameterValueToString(parameter);
    }

    int IdonethDeepkinBase::EnumStringToInt(const std::string &enumString) {
        if (enumString == "None") return None;
        else if (enumString == "Ionrach") return Ionrach;
        else if (enumString == "Dhom-hain") return DhomHain;
        else if (enumString == "Fuethan") return Fuethan;
        else if (enumString == "Mor'phann") return Morphann;
        else if (enumString == "Nautilar") return Nautilar;
        else if (enumString == "Briomdar") return Briomdar;
        return 0;
    }

    void IdonethDeepkinBase::setEnclave(Enclave enclave) {
        removeKeyword(IONRACH);
        removeKeyword(DHOMHAIN);
        removeKeyword(FUETHAN);
        removeKeyword(MORPHANN);
        removeKeyword(NAUTILAR);
        removeKeyword(BRIOMDAR);

        m_enclave = enclave;
        switch (enclave) {
            case Ionrach:
                addKeyword(IONRACH);
                break;
            case DhomHain:
                addKeyword(DHOMHAIN);
                break;
            case Fuethan:
                addKeyword(FUETHAN);
                break;
            case Morphann:
                addKeyword(MORPHANN);
                break;
            case Nautilar:
                addKeyword(NAUTILAR);
                break;
            case Briomdar:
                addKeyword(BRIOMDAR);
                break;
            default:
                break;
        }
    }

    void Init() {
        NamartiThralls::Init();
        NamartiReavers::Init();
        AkhelianMorrsarrGuard::Init();
        AkhelianIshlaenGuard::Init();
        AkhelianLeviadon::Init();
        AkhelianKing::Init();
        AkhelianAllopexes::Init();
        Volturnos::Init();
        EidolonOfMathlannAspectOfTheSea::Init();
        EidolonOfMathlannAspectOfTheStorm::Init();
        Lotann::Init();
        IsharannSoulscryer::Init();
        IsharannSoulrender::Init();
        IsharannTidecaster::Init();
    }

} //namespace IdonethDeepkin
