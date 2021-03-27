/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Fyreslayer.h>
#include <magic_enum.hpp>
#include <Board.h>
#include <Roster.h>

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
#include "fyreslayers/FjulGrimnir.h"
#include "fyreslayers/TheChosenAxes.h"

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
        FjulGrimnir::Init();
        TheChosenAxes::Init();
    }

    // Sorted in the order the runes are used.
    constexpr std::array<Rune, 6> g_runes = {
            Rune::Of_Relentless_Zeal,
            Rune::Of_Farsight,
            Rune::Of_Fury,
            Rune::Of_Searing_Heat,
            Rune::Of_Awakened_Steel,
            Rune::Of_Fiery_Determination
    };

    Rune Fyreslayer::s_activeRune = Rune::None;
    bool Fyreslayer::s_enhancedRuneActive = false;
    std::map<Rune, bool> Fyreslayer::s_availableRunes;

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
        if (s_enhancedRuneActive && (s_activeRune == Rune::Of_Relentless_Zeal)) {
            modifier += 2;
        }
        auto general = dynamic_cast<Fyreslayer *>(getRoster()->getGeneral());
        if (general && (distanceTo(general) < 18.0) &&
            (general->m_commandTrait == CommandTrait::Fury_Of_The_Fyreslayers)) {
            modifier += 1;
        }
        if (hasKeyword(LOFNIR) && hasKeyword(MAGMADROTH)) {
            if (general && (distanceTo(general) < 12.0) &&
                (general->m_commandTrait == CommandTrait::Explosive_Charge)) {
                modifier += 1;
            }
        }
        return modifier;
    }

    int Fyreslayer::rollRunDistance() {
        // Fearsome Surge
        if ((m_lodge == Lodge::Vostarg) && (m_battleRound == 1)) {
            return 6;
        }
        return Unit::rollRunDistance();
    }

    std::string Fyreslayer::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lodge") {
            auto lodgeName = magic_enum::enum_name((Lodge) parameter.intValue);
            return std::string(lodgeName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        }
        if (std::string(parameter.name) == "Mount Trait") {
            auto traitName = magic_enum::enum_name((MountTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Prayer") {
            auto prayerName = magic_enum::enum_name((Blessing) parameter.intValue);
            return std::string(prayerName);
        }

        return ParameterValueToString(parameter);
    }

    int Fyreslayer::EnumStringToInt(const std::string &enumString) {
        auto lodge = magic_enum::enum_cast<Lodge>(enumString);
        if (lodge.has_value()) return (int) lodge.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto mount = magic_enum::enum_cast<MountTrait>(enumString);
        if (mount.has_value()) return (int) mount.value();

        auto prayer = magic_enum::enum_cast<Blessing>(enumString);
        if (prayer.has_value()) return (int) prayer.value();

        return 0;
    }

    void Fyreslayer::activateRune() {
        // Select a rune activate
        for (auto rune : g_runes) {
            if (s_availableRunes[rune]) {
                auto roll = Dice::RollD6();

                auto general = dynamic_cast<Fyreslayer *>(getRoster()->getGeneral());
                if (general && (general->remainingModels() > 0) &&
                    (general->m_commandTrait == CommandTrait::Spirit_Of_Grimnir)) {
                    roll++;
                }

                if (roll >= 5) {
                    s_activeRune = rune;
                    s_enhancedRuneActive = (roll >= 6);
                    s_availableRunes[rune] = false;
                }
            }
        }
    }

    void Fyreslayer::onEndRound(int battleRound) {
        Unit::onEndRound(battleRound);

        s_activeRune = Rune::None;
    }

    void Fyreslayer::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (s_activeRune == Rune::None) {
            activateRune();

            // Only applied once
            if (s_enhancedRuneActive && (s_activeRune == Rune::Of_Searing_Heat)) {
                auto units = Board::Instance()->getAllUnits(GetEnemyId(owningPlayer()));
                for (auto unit : units) {
                    auto fs = Board::Instance()->getUnitsWithin(unit, owningPlayer(), 3.0);
                    if (!fs.empty()) {
                        unit->applyDamage({0, 1, Wounds::Source::Ability}, fs.front());
                    }
                }
            }
        }
    }

    void Fyreslayer::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void Fyreslayer::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Fyreslayer::onRestore() {
        Unit::onRestore();

        s_activeRune = Rune::None;
        s_enhancedRuneActive = false;
        for (auto rune : g_runes) {
            s_availableRunes[rune] = true;
        }
    }

    Rerolls Fyreslayer::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (s_activeRune == Rune::Of_Fury) {
            return Rerolls::Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int Fyreslayer::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        if ((s_activeRune == Rune::Of_Fury) && s_enhancedRuneActive) {
            attacks++;
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Battle_Scarred_Veteran)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            int numModels = 0;
            for (auto unit : units) {
                numModels += unit->remainingModels();
            }
            if (numModels >= 5) {
                attacks++;
            }
        }
        return attacks;
    }

    Wounds Fyreslayer::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        if (s_activeRune == Rune::Of_Searing_Heat) {
            if (hitRoll == 6) damage.normal++;
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Destroyer_Of_Foes) && (weapon->isMelee())) {
            damage.normal++;
        }
        return damage;
    }

    int Fyreslayer::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);
        if ((s_activeRune == Rune::Of_Awakened_Steel) && weapon->isMelee()) {
            rend--;
            if (s_enhancedRuneActive) rend--;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Fyresteel_Weaponsmith)) {
            rend--;
        }
        return rend;
    }

    int Fyreslayer::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (s_activeRune == Rune::Of_Fiery_Determination) {
            mod++;
        }

        auto general = dynamic_cast<Fyreslayer *>(getRoster()->getGeneral());
        if (general && (distanceTo(general) < 18.0) && hasKeyword(DUARDIN) &&
            (general->m_commandTrait == CommandTrait::Oathsayer)) {
            mod++;
        }
        return mod;
    }

    bool Fyreslayer::battleshockRequired() const {
        if (s_enhancedRuneActive && (s_activeRune == Rune::Of_Fiery_Determination)) {
            return false;
        }

        auto general = dynamic_cast<Fyreslayer *>(getRoster()->getGeneral());
        if (general && (distanceTo(general) < 12.0) &&
            (general->m_commandTrait == CommandTrait::Honour_Of_The_Ancestors)) {
            return false;
        }
        return Unit::battleshockRequired();
    }

    int Fyreslayer::moveModifier() const {
        auto mod = Unit::moveModifier();
        if (s_activeRune == Rune::Of_Relentless_Zeal) {
            mod += 2;
        }
        return mod;
    }

    int Fyreslayer::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        if (s_activeRune == Rune::Of_Farsight) {
            mod++;
        }
        return mod;
    }

    int Fyreslayer::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        if (s_enhancedRuneActive && (s_activeRune == Rune::Of_Farsight)) {
            mod++;
        }
        return mod;
    }

    int Fyreslayer::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);

        if (isGeneral() && (m_commandTrait == CommandTrait::Iron_Will_Of_The_Guardian)) {
            mod++;
        }
        return mod;
    }

    void Fyreslayer::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        if (isGeneral() && (m_commandTrait == CommandTrait::Wisdom_And_Authority) && (battleRound == 1)) {
            getRoster()->addCommandPoints(Dice::RollD3());
        }
    }

    void Fyreslayer::onStartMovement(PlayerId player) {
        Unit::onStartMovement(player);

        if (owningPlayer() == player) {
            if (isGeneral() && (m_commandTrait == CommandTrait::Fiery_Endurance)) {
                buffMovement(MovementRule::Run_And_Charge, true, {Phase::Combat, m_battleRound, owningPlayer()});
            } else {
                auto general = dynamic_cast<Fyreslayer *>(getRoster()->getGeneral());
                if (general && (general->m_commandTrait == CommandTrait::Fiery_Endurance) &&
                    distanceTo(general) < 12.0) {
                    buffMovement(MovementRule::Run_And_Charge, true, {Phase::Combat, m_battleRound, owningPlayer()});
                }
            }
        }
    }

    int Fyreslayer::targetWoundModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetWoundModifier(weapon, attacker);

        if (isGeneral() && (m_commandTrait == CommandTrait::Warrior_Indominate)) {
            mod--;
        } else if (hasKeyword(HERMDAR)) {
            auto general = dynamic_cast<Fyreslayer *>(getRoster()->getGeneral());
            if (general && (general->m_commandTrait == CommandTrait::Warrior_Indominate) &&
                distanceTo(general) < 12.0) {
                mod--;
            }
        }
        return mod;
    }

    int Fyreslayer::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        if (isGeneral() && (m_commandTrait == CommandTrait::Fyremantle) && (distanceTo(attacker) < 3.0)) {
            mod--;
        }
        return mod;
    }

} // namespace Fyreslayers
