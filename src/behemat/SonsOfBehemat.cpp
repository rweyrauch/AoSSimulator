/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <behemat/SonsOfBehemat.h>
#include <behemat/Mancrusher.h>
#include <behemat/Gatebreaker.h>
#include <behemat/KrakenEater.h>
#include <behemat/Warstomper.h>
#include <behemat/Bonegrinder.h>
#include "../AoSSimPrivate.h"
#include <magic_enum.hpp>

namespace SonsOfBehemat {

    std::string SonsOfBehematBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait)parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact)parameter.intValue);
            return std::string(artefactName);
        }
        return ParameterValueToString(parameter);
    }

    int SonsOfBehematBase::EnumStringToInt(const std::string &enumString) {

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        return 0;
    }

    void SonsOfBehematBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void SonsOfBehematBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Init() {
        Mancrusher::Init();
        Gatebreaker::Init();
        KrakenEater::Init();
        Warstomper::Init();
        Bonegrinder::Init();
    }

} //namespace SonsOfBehemat

