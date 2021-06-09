/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/IdonethDeepkin.h>
#include <magic_enum.hpp>
#include "idonethdeepkin/AkhelianAllopexes.h"
#include "idonethdeepkin/AkhelianKing.h"
#include "idonethdeepkin/AkhelianLeviadon.h"
#include "idonethdeepkin/EidolonSea.h"
#include "idonethdeepkin/EidolonStorm.h"
#include "idonethdeepkin/IsharannSoulrender.h"
#include "idonethdeepkin/IsharannSoulscryer.h"
#include "idonethdeepkin/IsharannTidecaster.h"
#include "idonethdeepkin/IshlaenGuard.h"
#include "idonethdeepkin/Lotann.h"
#include "idonethdeepkin/MorrsarrGuard.h"
#include "idonethdeepkin/NamartiReavers.h"
#include "idonethdeepkin/NamartiThralls.h"
#include "idonethdeepkin/Volturnos.h"

namespace IdonethDeepkin {

    std::string IdonethDeepkinBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Enclave") {
            auto enclaveName = magic_enum::enum_name((Enclave) parameter.intValue);
            return std::string(enclaveName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        }
        if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore) parameter.intValue);
            return std::string(loreName);
        }
        if (std::string(parameter.name) == "Mount Trait") {
            auto traitName = magic_enum::enum_name((MountTrait) parameter.intValue);
            return std::string(traitName);
        }
        return ParameterValueToString(parameter);
    }

    int IdonethDeepkinBase::EnumStringToInt(const std::string &enumString) {
        auto enclave = magic_enum::enum_cast<Enclave>(enumString);
        if (enclave.has_value()) return (int) enclave.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto mtrait = magic_enum::enum_cast<MountTrait>(enumString);
        if (mtrait.has_value()) return (int) mtrait.value();

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
            case Enclave::Ionrach:
                addKeyword(IONRACH);
                break;
            case Enclave::Dhom_Hain:
                addKeyword(DHOMHAIN);
                break;
            case Enclave::Fuethan:
                addKeyword(FUETHAN);
                break;
            case Enclave::Morphann:
                addKeyword(MORPHANN);
                break;
            case Enclave::Nautilar:
                addKeyword(NAUTILAR);
                break;
            case Enclave::Briomdar:
                addKeyword(BRIOMDAR);
                break;
            default:
                break;
        }
    }

    void IdonethDeepkinBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void IdonethDeepkinBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Init() {
        AkhelianAllopexes::Init();
        AkhelianIshlaenGuard::Init();
        AkhelianKing::Init();
        AkhelianLeviadon::Init();
        AkhelianMorrsarrGuard::Init();
        EidolonOfMathlannAspectOfTheSea::Init();
        EidolonOfMathlannAspectOfTheStorm::Init();
        IsharannSoulrender::Init();
        IsharannSoulscryer::Init();
        IsharannTidecaster::Init();
        Lotann::Init();
        NamartiReavers::Init();
        NamartiThralls::Init();
        Volturnos::Init();
    }

} //namespace IdonethDeepkin
