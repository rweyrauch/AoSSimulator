/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/DaughterOfKhaine.h>

#include "dok/WitchAelves.h"
#include "dok/BloodSisters.h"
#include "dok/KhineraiLifetakers.h"
#include "dok/SistersOfSlaughter.h"
#include "dok/KhineraiHeartrenders.h"
#include "dok/DoomfireWarlocks.h"
#include "dok/BloodStalkers.h"
#include "dok/HagQueen.h"
#include "dok/SlaughterQueen.h"
#include "dok/AvatarOfKhaine.h"
#include "dok/BloodwrackMedusa.h"
#include "dok/BloodwrackShrine.h"
#include "dok/HagQueenCauldronOfBlood.h"
#include "dok/MorathiHighOracle.h"
#include "dok/MorathiShadowQueen.h"
#include "dok/SlaughterQueenCauldronOfBlood.h"

namespace DaughtersOfKhaine {


    int DaughterOfKhaine::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = Unit::toHitModifier(weapon, unit);

        // Bladed Killers
        if (hasKeyword(DRAICHI_GANETH) && m_charged) {
            modifier += 1;
        }
        return modifier;
    }


    Rerolls DaughterOfKhaine::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Blood Rites - Zealot's Rage
        if (m_battleRound == 3) {
            // Daughters of the First Temple
            if (hasKeyword(HAGG_NAR)) {
                return RerollFailed;
            }
            return RerollOnes;
        }
        return Unit::toHitRerolls(weapon, unit);
    }

    Rerolls DaughterOfKhaine::toWoundRerolls(const Weapon *weapon, const Unit *unit) const {
        // Blood Rites - Slaughterer's Strength
        if (m_battleRound == 4) {
            return RerollOnes;
        }

        return Unit::toWoundRerolls(weapon, unit);
    }

    Rerolls DaughterOfKhaine::toSaveRerolls(const Weapon *weapon) const {
        // Blood Rites - Unquenchable Fervour
        if (m_battleRound == 5) {
            return RerollOnes;
        }

        return Unit::toSaveRerolls(weapon);
    }

    Rerolls DaughterOfKhaine::runRerolls() const {
        // Blood Rites - Quickening Bloodlust
        if (m_battleRound == 1) {
            return RerollOnes;
        }
        return Unit::runRerolls();
    }

    Rerolls DaughterOfKhaine::chargeRerolls() const {
        // Blood Rites - Headlong Fury
        if (m_battleRound == 2) {
            return RerollOnes;
        }
        return Unit::chargeRerolls();
    }

    int DaughterOfKhaine::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);

        // Concealment and Stealth
        if (hasKeyword(KHAILEBRON) && weapon->isMissile()) mod--;

        return mod;
    }

    Wounds DaughterOfKhaine::applyWoundSave(const Wounds &wounds) {
        // Fanatical Faith
        Dice::RollResult mortalSaves, normalSaves;
        Dice::rollD6(wounds.mortal, mortalSaves);
        Dice::rollD6(wounds.normal, normalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= normalSaves.rollsGE(6);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(6);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    std::string DaughterOfKhaine::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Temple") {
            if (parameter.intValue == HaggNar) { return "Hagg Nar"; }
            else if (parameter.intValue == DraichiGaneth) { return "Draichi Ganeth"; }
            else if (parameter.intValue == TheKraith) { return "The Kraith"; }
            else if (parameter.intValue == Khailebron) { return "Khailebron"; }
            else if (parameter.intValue == Custom) return "Custom";
        }
        return ParameterValueToString(parameter);
    }

    int DaughterOfKhaine::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Hagg Nar") return HaggNar;
        else if (enumString == "Draichi Ganeth") return DraichiGaneth;
        else if (enumString == "The Kraith") return TheKraith;
        else if (enumString == "Khailebron") return Khailebron;
        else if (enumString == "Custom") return Custom;
        return 0;
    }

    void DaughterOfKhaine::setTemple(Temple temple) {
        removeKeyword(HAGG_NAR);
        removeKeyword(DRAICHI_GANETH);
        removeKeyword(THE_KRAITH);
        removeKeyword(KHAILEBRON);

        m_temple = temple;
        switch (temple) {
            case HaggNar:
                addKeyword(HAGG_NAR);
                break;
            case DraichiGaneth:
                addKeyword(DRAICHI_GANETH);
                break;
            case TheKraith:
                addKeyword(THE_KRAITH);
                break;
            case Khailebron:
                addKeyword(KHAILEBRON);
                break;
            default:
                break;
        }
    }

    void Init() {
        AvatarOfKhaine::Init();
        BloodSisters::Init();
        BloodStalkers::Init();
        BloodwrackMedusa::Init();
        BloodwrackShrine::Init();
        DoomfireWarlocks::Init();
        HagQueen::Init();
        HagQueenOnCauldronOfBlood::Init();
        KhineraiHeartrenders::Init();
        KhineraiLifetakers::Init();
        MorathiHighOracleOfKhaine::Init();
        MorathiTheShadowQueen::Init();
        SistersOfSlaughter::Init();
        SlaughterQueen::Init();
        SlaughterQueenOnCauldronOfBlood::Init();
        WitchAelves::Init();
    }

} // namespace DaughtersOfKhaine
