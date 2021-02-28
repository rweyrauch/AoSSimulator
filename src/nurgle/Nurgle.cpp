/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <magic_enum.hpp>
#include <nurgle/Nurgle.h>

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
#include "nurgle/SorcererOfNurgle.h"

namespace Nurgle {

    std::string NurgleBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Plague Legion") {
            auto legionName = magic_enum::enum_name((PlagueLegion)parameter.intValue);
            return std::string(legionName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait)parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact)parameter.intValue);
            return std::string(artefactName);
        }
        if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore)parameter.intValue);
            return std::string(loreName);
        }
        return ParameterValueToString(parameter);
    }

    int NurgleBase::EnumStringToInt(const std::string &enumString) {
        auto legion = magic_enum::enum_cast<PlagueLegion>(enumString);
        if (legion.has_value()) return (int)legion.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        return 0;
    }

    void NurgleBase::setLegion(PlagueLegion legion) {
        removeKeyword(MUNIFICENT_WANDERERS);
        removeKeyword(DRONING_GUARD);
        removeKeyword(BLESSED_SONS);
        removeKeyword(DROWNED_MEN);

        m_plagueLegion = legion;
        switch (m_plagueLegion) {
            case PlagueLegion::Munificent_Wanderers:
                addKeyword(MUNIFICENT_WANDERERS);
                break;
            case PlagueLegion::Drowning_Guard:
                addKeyword(DRONING_GUARD);
                break;
            case PlagueLegion::Blessed_Sons:
                addKeyword(BLESSED_SONS);
                break;
            case PlagueLegion::Drowned_Men:
                addKeyword(DROWNED_MEN);
                break;
            default:
                break;
        }
    }

    void Init() {
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
        SorcererOfNurgle::Init();
        MorbidexTwiceborn::Init();
    }

} //namespace Nurgle
