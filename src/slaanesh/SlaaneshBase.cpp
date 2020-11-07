/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <magic_enum.hpp>
#include <slaanesh/SlaaneshBase.h>
#include "slaanesh/Fiends.h"
#include "slaanesh/Seekers.h"
#include "slaanesh/Daemonettes.h"
#include "slaanesh/KeeperOfSecrets.h"
#include "slaanesh/Hellstriders.h"
#include "slaanesh/ShalaxiHelbane.h"
#include "slaanesh/BladebringerExaltedChariot.h"
#include "slaanesh/BladebringerHellflayer.h"
#include "slaanesh/BladebringerSeekerChariot.h"
#include "slaanesh/ExaltedChariot.h"
#include "slaanesh/Hellflayer.h"
#include "slaanesh/SeekerChariots.h"
#include "slaanesh/TheMasque.h"
#include "slaanesh/Viceleader.h"
#include "slaanesh/InfernalEnrapturess.h"
#include "slaanesh/SyllEsske.h"
#include "slaanesh/ContortedEpitome.h"
#include "slaanesh/LordOfPain.h"

namespace Slaanesh {

    void SlaaneshBase::setHost(Host host) {
        removeKeyword(INVADERS);
        removeKeyword(PRETENDERS);
        removeKeyword(GODSEEKERS);

        m_host = host;
        switch (m_host) {
            case Host::Invaders:
                addKeyword(INVADERS);
                break;
            case Host::Pretenders:
                addKeyword(PRETENDERS);
                break;
            case Host::Godseekers:
                addKeyword(GODSEEKERS);
                break;
            default:
                break;
        }
    }

    std::string SlaaneshBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Host") {
            auto hostName = magic_enum::enum_name((Host)parameter.intValue);
            return std::string(hostName);
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

    int SlaaneshBase::EnumStringToInt(const std::string &enumString) {
        auto host = magic_enum::enum_cast<Host>(enumString);
        if (host.has_value()) return (int)host.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        return 0;
    }

    int SlaaneshBase::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        int hits = Unit::generateHits(unmodifiedHitRoll, weapon, unit);

        // Euphoric Killers
        if ((unmodifiedHitRoll == 6) && !weapon->isMissile()) {
            hits += 1;
            if (remainingModels() >= 20) {
                hits += 1;
            }
        }
        return hits;
    }

    Rerolls SlaaneshBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Heir to the Throne
        if ((m_host == Host::Pretenders) && (remainingModels() >= 10)) {
            return RerollOnes;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int SlaaneshBase::chargeModifier() const {
        int modifier = Unit::chargeModifier();

        // Thundering Cavalcade
        if (m_host == Host::Godseekers) {
            modifier += 1;
        }
        return modifier;
    }

    void SlaaneshBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void SlaaneshBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Init() {
        Daemonettes::Init();
        Fiends::Init();
        Seekers::Init();
        KeeperOfSecrets::Init();
        Hellstriders::Init();
        ShalaxiHelbane::Init();
        BladebringerOnExaltedChariot::Init();
        BladebringerOnHellflayer::Init();
        BladebringerOnSeekerChariot::Init();
        ExaltedChariot::Init();
        Hellflayer::Init();
        SeekerChariots::Init();
        TheMasque::Init();
        ViceleaderHeraldOfSlaanesh::Init();
        InfernalEnrapturess::Init();
        SyllEsske::Init();
        TheContortedEpitome::Init();
        LordOfPain::Init();
    }

} // namespace Slaanesh
