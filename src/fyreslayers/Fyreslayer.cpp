/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Fyreslayer.h>
#include <magic_enum.hpp>

#include "fyreslayers/AuricHearthguard.h"
#include "fyreslayers/Battlesmith.h"
#include "fyreslayers/Doomseeker.h"
#include "fyreslayers/HearthguardBerzerkers.h"
#include "fyreslayers/GrimwrathBerzerker.h"
#include "fyreslayers/VulkiteBerzerkers.h"
#include "fyreslayers/AuricRunefather.h"
#include "fyreslayers/AuricRunemaster.h"
#include "fyreslayers/AuricRuneson.h"
#include "fyreslayers/AuricRunesmiterOnMagmadroth.h"
#include "fyreslayers/AuricRunesonOnMagmadroth.h"
#include "fyreslayers/AuricRunefatherOnMagmadroth.h"
#include "fyreslayers/AuricRunesmiter.h"

namespace Fyreslayers {

    void Init() {
        AuricHearthguard::Init();
        Battlesmith::Init();
        GrimwrathBerzerker::Init();
        HearthguardBerzerkers::Init();
        Doomseeker::Init();
        VulkiteBerzerkers::Init();
        AuricRunefather::Init();
        AuricRunemaster::Init();
        AuricRuneson::Init();
        AuricRunesmiter::Init();
        AuricRunesmiterOnMagmadroth::Init();
        AuricRunefatherOnMagmadroth::Init();
        AuricRunesonOnMagmadroth::Init();
    }

    void Fyreslayer::setLodge(Lodge lodge) {
        removeKeyword(VOSTARG);
        removeKeyword(GREYFYRD);
        removeKeyword(HERMDAR);
        removeKeyword(LOFNIR);

        m_lodge = lodge;
        switch (m_lodge) {
            case Lodge::Vostarg:
                addKeyword(VOSTARG);
                break;
            case Lodge::Greyfyrd:
                addKeyword(GREYFYRD);
                break;
            case Lodge::Hermdar:
                addKeyword(HERMDAR);
                break;
            case Lodge::Lofnir:
                addKeyword(LOFNIR);
                break;
            default:
                break;
        }
    }

    int Fyreslayer::chargeModifier() const {
        auto modifier = Unit::chargeModifier();

        // Fearsome Surge
        if (m_lodge == Lodge::Vostarg) {
            modifier += 1;
        }

        return modifier;
    }

    int Fyreslayer::rollRunDistance() const {
        // Fearsome Surge
        if ((m_lodge == Lodge::Vostarg) && (m_battleRound == 1)) {
            return 6;
        }
        return Unit::rollRunDistance();
    }

    std::string Fyreslayer::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lodge") {
            auto lodgeName = magic_enum::enum_name((Lodge)parameter.intValue);
            return std::string(lodgeName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait)parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact)parameter.intValue);
            return std::string(artefactName);
        }
        if (std::string(parameter.name) == "Mount Trait") {
            auto traitName = magic_enum::enum_name((MountTrait)parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Prayer") {
            auto prayerName = magic_enum::enum_name((Blessing)parameter.intValue);
            return std::string(prayerName);
        }

        return ParameterValueToString(parameter);
    }

    int Fyreslayer::EnumStringToInt(const std::string &enumString) {
        auto lodge = magic_enum::enum_cast<Lodge>(enumString);
        if (lodge.has_value()) return (int)lodge.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        auto mount = magic_enum::enum_cast<MountTrait>(enumString);
        if (mount.has_value()) return (int)mount.value();

        auto prayer = magic_enum::enum_cast<Blessing>(enumString);
        if (prayer.has_value()) return (int)prayer.value();

        return 0;
    }

    void Fyreslayer::activateRune() {
        // Select a rune activate
    }

    void Fyreslayer::onEndRound(int battleRound) {
        Unit::onEndRound(battleRound);

        m_activatedRune = false;
    }

    void Fyreslayer::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (!m_activatedRune) {
            activateRune();
            m_activatedRune = true;
        }
    }

    void Fyreslayer::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void Fyreslayer::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

} // namespace Fyreslayers
