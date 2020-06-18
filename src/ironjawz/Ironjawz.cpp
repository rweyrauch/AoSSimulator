/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Roster.h>
#include <ironjawz/Ironjawz.h>
#include <magic_enum.hpp>

#include "ironjawz/OrrukArdboys.h"
#include "ironjawz/OrrukBrutes.h"
#include "ironjawz/GoreGruntas.h"
#include "ironjawz/OrrukMegaboss.h"
#include "ironjawz/OrrukWarchanter.h"
#include "ironjawz/Gordrakk.h"
#include "ironjawz/MegabossOnMawKrusha.h"
#include "ironjawz/IronskullsBoyz.h"
#include "ironjawz/WeirdnobShaman.h"

namespace Ironjawz {

    int Ironjawz::chargeModifier() const {
        int mod = Unit::chargeModifier();

        // Eager for Battle
        mod++;

        // Hunt and Crush
        if (m_warclan == Warclan::Bloodtoofs) {
            mod++;
        }
        return mod;
    }

    int Ironjawz::runModifier() const {
        int mod = Unit::runModifier();

        // Hunt and Crush
        if (m_warclan == Warclan::Bloodtoofs) {
            mod++;
        }
        return mod;
    }

    void Ironjawz::setWarclan(Warclan warclan) {
        removeKeyword(IRONSUNZ);
        removeKeyword(BLOODTOOFS);
        removeKeyword(CHOPPAS);

        m_warclan = warclan;
        switch (warclan) {
            case Warclan::Ironsunz:
                addKeyword(IRONSUNZ);
                break;
            case Warclan::Bloodtoofs:
                addKeyword(BLOODTOOFS);
                break;
            case Warclan::Da_Choppas:
                addKeyword(CHOPPAS);
                break;
            default:
                break;
        }
    }

    std::string Ironjawz::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Warclan") {
            auto clanName = magic_enum::enum_name((Warclan)parameter.intValue);
            return std::string(clanName);
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
        if (std::string(parameter.name) == "Mount Trait") {
            auto traitName = magic_enum::enum_name((MountTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Warbeat") {
            auto beatName = magic_enum::enum_name((Warbeat) parameter.intValue);
            return std::string(beatName);
        }

        return ParameterValueToString(parameter);
    }

    int Ironjawz::EnumStringToInt(const std::string &enumString) {
        auto clan = magic_enum::enum_cast<Warclan>(enumString);
        if (clan.has_value()) return (int)clan.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        auto mount = magic_enum::enum_cast<MountTrait>(enumString);
        if (mount.has_value()) return (int)mount.value();

        auto beat = magic_enum::enum_cast<Warbeat>(enumString);
        if (beat.has_value()) return (int)beat.value();

        return 0;
    }

    void Ironjawz::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        // Right First of Dakkbad
        if ((battleRound == 1) && (m_warclan == Warclan::Ironsunz))
            m_roster->addCommandPoints(1);
    }

    void Ironjawz::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void Ironjawz::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Init() {
        OrrukArdboys::Init();
        OrrukBrutes::Init();
        OrrukGoreGruntas::Init();
        OrrukMegaboss::Init();
        OrrukWarchanter::Init();
        GordrakkTheFistOfGork::Init();
        MegabossOnMawKrusha::Init();
        IronskullsBoyz::Init();
        OrrukWeirdnobShaman::Init();
    }

} // namespace Ironjawz

