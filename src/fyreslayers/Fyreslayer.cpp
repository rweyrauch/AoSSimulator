/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Fyreslayer.h>

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

    void Fyreslayer::setLodge(Fyreslayers::Lodge lodge) {
        removeKeyword(VOSTARG);
        removeKeyword(GREYFYRD);
        removeKeyword(HERMDAR);
        removeKeyword(LOFNIR);

        m_lodge = lodge;
        switch (m_lodge) {
            case Vostarg:
                addKeyword(VOSTARG);
                break;
            case Greyfyrd:
                addKeyword(GREYFYRD);
                break;
            case Hermdar:
                addKeyword(HERMDAR);
                break;
            case Lofnir:
                addKeyword(LOFNIR);
                break;
            default:
                break;
        }
    }

    int Fyreslayer::chargeModifier() const {
        auto modifier = Unit::chargeModifier();

        // Fearsome Surge
        if (m_lodge == Vostarg) {
            modifier += 1;
        }

        return modifier;
    }

    int Fyreslayer::rollRunDistance() const {
        // Fearsome Surge
        if ((m_lodge == Vostarg) && (m_battleRound == 1)) {
            return 6;
        }
        return Unit::rollRunDistance();
    }

    std::string Fyreslayer::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lodge") {
            if (parameter.intValue == Vostarg) { return "Vostarg"; }
            else if (parameter.intValue == Greyfyrd) { return "Greyfyrd"; }
            else if (parameter.intValue == Hermdar) { return "Hermdar"; }
            else if (parameter.intValue == Lofnir) { return "Lofnir"; }
            else if (parameter.intValue == Custom) { return "Custom"; }
        }
        return ParameterValueToString(parameter);
    }

    int Fyreslayer::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Vostarg") { return Vostarg; }
        else if (enumString == "Greyfyrd") { return Greyfyrd; }
        else if (enumString == "Hermdar") { return Hermdar; }
        else if (enumString == "Lofnir") { return Lofnir; }
        else if (enumString == "Custom") { return Custom; }
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

} // namespace Fyreslayers
