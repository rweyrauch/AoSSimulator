/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <magic_enum.hpp>
#include "Roster.h"
#include <kharadron/KharadronBase.h>
#include <kharadron/AethericNavigator.h>
#include <kharadron/AetherKhemist.h>
#include <kharadron/ArkanautAdmiral.h>
#include <kharadron/ArkanautCompany.h>
#include <kharadron/ArkanautFrigate.h>
#include <kharadron/ArkanautIronclad.h>
#include <kharadron/BrokkGrungsson.h>
#include <kharadron/EndrinmasterWithHarness.h>
#include <kharadron/EndrinmasterWithSuit.h>
#include <kharadron/Endrinriggers.h>
#include <kharadron/GrundstokGunhauler.h>
#include <kharadron/GrundstokThunderers.h>
#include <kharadron/Skywardens.h>
#include <Board.h>
#include "kharadron/BjorgenThundrik.h"
#include "kharadron/ThundriksProfiteers.h"

namespace KharadronOverlords {

    bool KharadronBase::s_usedFootnote = false;

    std::string KharadronBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Skyport") {
            auto portName = magic_enum::enum_name((Skyport) parameter.intValue);
            return std::string(portName);
        }
        if (std::string(parameter.name) == "Artycle") {
            auto artycleName = magic_enum::enum_name((Artycle) parameter.intValue);
            return std::string(artycleName);
        }
        if (std::string(parameter.name) == "Amendment") {
            auto amendmentName = magic_enum::enum_name((Amendment) parameter.intValue);
            return std::string(amendmentName);
        }
        if (std::string(parameter.name) == "Footnote") {
            auto noteName = magic_enum::enum_name((Footnote) parameter.intValue);
            return std::string(noteName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        }

        return ParameterValueToString(parameter);
    }

    int KharadronBase::EnumStringToInt(const std::string &enumString) {
        auto port = magic_enum::enum_cast<Skyport>(enumString);
        if (port.has_value()) return (int) port.value();

        auto artycle = magic_enum::enum_cast<Artycle>(enumString);
        if (artycle.has_value()) return (int) artycle.value();

        auto amendment = magic_enum::enum_cast<Amendment>(enumString);
        if (amendment.has_value()) return (int) amendment.value();

        auto note = magic_enum::enum_cast<Footnote>(enumString);
        if (note.has_value()) return (int) note.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        return 0;
    }

    void KharadronBase::setSkyport(Skyport skyport) {
        removeKeyword(BARAK_NAR);
        removeKeyword(BARAK_ZON);
        removeKeyword(BARAK_ZILFIN);
        removeKeyword(BARAK_URBAZ);
        removeKeyword(BARAK_THRYNG);
        removeKeyword(BARAK_MHORNAR);

        m_skyport = skyport;
        switch (skyport) {
            case Skyport::Barak_Nar:
                addKeyword(BARAK_NAR);
                setCode(Artycle::Respect_Your_Commanders, Amendment::Trust_Aethermatics_Not_Superstition,
                        Footnote::Through_Knowledge_Power);
                break;
            case Skyport::Barak_Zon:
                addKeyword(BARAK_ZON);
                setCode(Artycle::Honour_Is_Everything, Amendment::Leave_No_Duardin_Behind,
                        Footnote::Show_Them_Your_Steel);
                break;
            case Skyport::Barak_Zilfin:
                addKeyword(BARAK_ZILFIN);
                setCode(Artycle::Master_The_Skies, Amendment::Dont_Argue_With_The_Wind,
                        Footnote::Theres_Always_A_Breeze);
                break;
            case Skyport::Barak_Urbaz:
                addKeyword(BARAK_URBAZ);
                setCode(Artycle::Seek_New_Prospects, Amendment::Always_Take_What_You_Are_Owed,
                        Footnote::Where_Theres_War_Theres_Gold);
                break;
            case Skyport::Barak_Thryng:
                addKeyword(BARAK_THRYNG);
                setCode(Artycle::Chronicle_Of_Grudges, Amendment::Take_Help_Where_You_Can_Get_It,
                        Footnote::Honour_The_Gods_Just_In_Case);
                break;
            case Skyport::Barak_Mhornar:
                addKeyword(BARAK_MHORNAR);
                setCode(Artycle::Seek_New_Prospects, Amendment::Prosecute_Wars_With_All_Haste,
                        Footnote::Who_Strikes_First_Strikes_Hardest);
                break;
            default:
                break;
        }
    }

    void KharadronBase::setCode(Artycle artycle, Amendment amendment,
                                Footnote footnote) {
        if (m_skyport != Skyport::None) {
            // Error.
        }
        m_artycle = artycle;
        m_amendment = amendment;
        m_footnote = footnote;
    }

    void KharadronBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void KharadronBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    Rerolls KharadronBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if ((m_artycle == Artycle::Honour_Is_Everything) && hasKeyword(HERO) &&
            (target->hasKeyword(HERO) || target->hasKeyword(MONSTER))) {
            return Rerolls::Ones;
        }
        if ((m_artycle == Artycle::Master_The_Skies) && hasKeyword(SKYVESSEL) && target->canFly()) {
            return Rerolls::Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int KharadronBase::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_amendment == Amendment::Trust_To_Your_Guns) {
            auto enemy = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (enemy && (distanceTo(enemy) < 3.0)) {
                mod++;
            }
            else if (enemy == nullptr) {
                mod++;
            }
        }
        if (hasKeyword(SKYFARERS) && (m_amendment == Amendment::Leave_No_Duardin_Behind)) {
            auto vessels = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto vessel : vessels) {
                if (vessel->hasKeyword(SKYVESSEL) && (vessel->remainingModels() > 0)) {
                    mod += 2;
                    break;
                }
            }
        }
        return mod;
    }

    void KharadronBase::onRestore() {
        Unit::onRestore();

        s_usedFootnote = false;
        m_aetherGold = 1;
    }

    void KharadronBase::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        if (isGeneral() && (battleRound == 1) && (m_commandTrait == CommandTrait::Wealthy)) {
            m_aetherGold++;
        }
    }

    int KharadronBase::woundModifier() const {
        auto mod = Unit::woundModifier();

        if (m_endrinwork == Endrinwork::Hegsson_Solutions_Old_Reliable_Hullplates) {
            mod += 2;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Tough_As_Old_Boots)) {
            mod += 2;
        }
        return mod;
    }

    void KharadronBase::setEndrinwork(Endrinwork endrinwork) {
        m_endrinwork = endrinwork;
    }

    int KharadronBase::moveModifier() const {
        auto mod = Unit::moveModifier();

        if (m_endrinwork == Endrinwork::Magnificent_Omniscope) {
            mod += 2;
        }
        return mod;
    }

    Rerolls KharadronBase::chargeRerolls() const {
        if (!s_usedFootnote && (m_footnote == Footnote::Theres_No_Reward_Without_Risk)) {
            s_usedFootnote = true;
            return Rerolls::Failed;
        }
        return Unit::chargeRerolls();
    }

    void KharadronBase::onEndShooting(PlayerId player) {
        Unit::onEndShooting(player);

        if (hasKeyword(SKYVESSEL) && (m_footnote == Footnote::Without_Our_Ships_We_Are_Naught) && !s_usedFootnote) {
            if (remainingWounds() < wounds()) {
                heal(Dice::RollD3());
                s_usedFootnote = true;
            }
        }
    }

    void KharadronBase::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        if (hasKeyword(SKYVESSEL) && (m_footnote == Footnote::Without_Our_Ships_We_Are_Naught) && !s_usedFootnote) {
            if (remainingWounds() < wounds()) {
                heal(Dice::RollD3());
                s_usedFootnote = true;
            }
        }
    }

    int KharadronBase::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Sceptic)) {
            mod++;
        }
        if ((m_footnote == Footnote::Through_Knowledge_Power) && (hasKeyword(HERO))) {
            mod++;
        }
        return mod;
    }

    void KharadronBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            if (isGeneral() && (m_commandTrait == CommandTrait::A_Nose_For_Gold)) {
                if (Dice::RollD6() >= 5) {
                    m_aetherGold++;
                }
            }
            if (isGeneral() && (m_commandTrait == CommandTrait::War_Wounds)) {
                if (Dice::RollD6() >= 2) {
                    getRoster()->addCommandPoints(1);
                }
                else {
                    buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                    buffModifier(Attribute::To_Hit_Missile, -1, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                }
            }
        }
    }

    int KharadronBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        if (isGeneral() && (m_commandTrait == CommandTrait::Great_Tinkerer)) {
            if (weapon->name() == "Gaze of Grungni") {
                attacks += 2;
            }
        }
        return attacks;
    }

    void KharadronBase::onBeginTurn(int battleRound, PlayerId player) {
        Unit::onBeginTurn(battleRound, player);

        if ((owningPlayer() == player) && (battleRound == 1) && (m_amendment == Amendment::Prosecute_Wars_With_All_Haste)) {
            buffMovement(MovementRule::Run_And_Shoot, true, {GamePhase::Hero, battleRound + 1, player});
        }
    }

    Rerolls KharadronBase::battleshockRerolls() const {
        if (hasKeyword(BARAK_NAR) && (m_artycle == Artycle::Respect_Your_Commanders)) {
            auto heroes = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto hero : heroes) {
                if (hero->hasKeyword(HERO) && (hero->remainingModels() > 0) && hero->hasKeyword(BARAK_NAR)) {
                    return Rerolls::Failed;
                }
            }
        }
        if (m_artycle == Artycle::Seek_New_Prospects) {
            if (Board::Instance()->isUnitWithinDeploymentZone(this, GetEnemyId(owningPlayer()))) {
                return Rerolls::Failed;
            }
        }
        return Unit::battleshockRerolls();
    }

    void Init() {
        AethericNavigator::Init();
        AetherKhemist::Init();
        ArkanautAdmiral::Init();
        ArkanautCompany::Init();
        ArkanautFrigate::Init();
        ArkanautIronclad::Init();
        BrokkGrungsson::Init();
        EndrinmasterWithDirigibleSuit::Init();
        EndrinmasterWithEndrinharness::Init();
        Endrinriggers::Init();
        GrundstokGunhauler::Init();
        GrundstokThunderers::Init();
        Skywardens::Init();
        BjorgenThundrik::Init();
        ThundriksProfiteers::Init();
    }

} //KharadronOverlords
