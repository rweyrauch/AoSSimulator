/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/DaughterOfKhaine.h>
#include <magic_enum.hpp>

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
#include "dok/Morgwaeth.h"
#include "dok/TheBladeCoven.h"
#include "dok/KhainiteShadowstalkers.h"
#include "dok/MelusaiIronscale.h"

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
                return Reroll_Failed;
            }
            return Reroll_Ones;
        }
        return Unit::toHitRerolls(weapon, unit);
    }

    Rerolls DaughterOfKhaine::toWoundRerolls(const Weapon *weapon, const Unit *unit) const {
        // Blood Rites - Slaughterer's Strength
        if (m_battleRound == 4) {
            return Reroll_Ones;
        }

        return Unit::toWoundRerolls(weapon, unit);
    }

    Rerolls DaughterOfKhaine::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Blood Rites - Unquenchable Fervour
        if (m_battleRound == 5) {
            return Reroll_Ones;
        }

        return Unit::toSaveRerolls(weapon, attacker);
    }

    Rerolls DaughterOfKhaine::runRerolls() const {
        // Blood Rites - Quickening Bloodlust
        if (m_battleRound == 1) {
            return Reroll_Ones;
        }
        return Unit::runRerolls();
    }

    Rerolls DaughterOfKhaine::chargeRerolls() const {
        // Blood Rites - Headlong Fury
        if (m_battleRound == 2) {
            return Reroll_Ones;
        }
        return Unit::chargeRerolls();
    }

    int DaughterOfKhaine::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);

        // Concealment and Stealth
        if (hasKeyword(KHAILEBRON) && weapon->isMissile()) mod--;

        return mod;
    }

    Wounds DaughterOfKhaine::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Fanatical Faith
        Dice::RollResult mortalSaves, normalSaves;
        Dice::RollD6(wounds.mortal, mortalSaves);
        Dice::RollD6(wounds.normal, normalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= normalSaves.rollsGE(6);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(6);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    std::string DaughterOfKhaine::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Temple") {
            auto templeName = magic_enum::enum_name((Temple)parameter.intValue);
            return std::string(templeName);
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
        if (std::string(parameter.name) == "Prayer") {
            auto prayerName = magic_enum::enum_name((Prayer)parameter.intValue);
            return std::string(prayerName);
        }
        return ParameterValueToString(parameter);
    }

    int DaughterOfKhaine::EnumStringToInt(const std::string &enumString) {
        auto temple = magic_enum::enum_cast<Temple>(enumString);
        if (temple.has_value()) return (int)temple.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        auto prayer = magic_enum::enum_cast<Prayer>(enumString);
        if (prayer.has_value()) return (int)prayer.value();

        return 0;
    }

    void DaughterOfKhaine::setTemple(Temple temple) {
        removeKeyword(HAGG_NAR);
        removeKeyword(DRAICHI_GANETH);
        removeKeyword(THE_KRAITH);
        removeKeyword(KHAILEBRON);
        removeKeyword(ZAINTHAR_KAI);

        m_temple = temple;
        switch (temple) {
            case Temple::Hagg_Nar:
                addKeyword(HAGG_NAR);
                break;
            case Temple::Draichi_Ganeth:
                addKeyword(DRAICHI_GANETH);
                break;
            case Temple::The_Kraith:
                addKeyword(THE_KRAITH);
                break;
            case Temple::Khailebron:
                addKeyword(KHAILEBRON);
                break;
            case Temple::Zainthar_Kai:
                addKeyword(ZAINTHAR_KAI);
                break;
            default:
                break;
        }
    }

    void DaughterOfKhaine::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void DaughterOfKhaine::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    int DaughterOfKhaine::braveryModifier() const {
        auto mod = Unit::braveryModifier();

        // Khaine's Essence
        if ((m_temple == Temple::Zainthar_Kai) && (hasKeyword(MELUSAI) || hasKeyword(KHINERAI_HARPIES))) {
            mod++;
        }
        return mod;
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
        KhainiteShadowstalkers::Init();
        KhineraiHeartrenders::Init();
        KhineraiLifetakers::Init();
        MelusaiIronscale::Init();
        MorathiHighOracleOfKhaine::Init();
        MorathiTheShadowQueen::Init();
        MorgwaethTheBloodied::Init();
        SistersOfSlaughter::Init();
        SlaughterQueen::Init();
        SlaughterQueenOnCauldronOfBlood::Init();
        TheBladeCoven::Init();
        WitchAelves::Init();
    }

} // namespace DaughtersOfKhaine
