/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <magic_enum.hpp>
#include <slaanesh/SlaaneshBase.h>
#include <Board.h>
#include <Roster.h>

#include "slaanesh/Fiends.h"
#include "slaanesh/Seekers.h"
#include "slaanesh/Daemonettes.h"
#include "slaanesh/KeeperOfSecrets.h"
#include "slaanesh/HellstridersWithClawspears.h"
#include "slaanesh/HellstridersWithHellscourges.h"
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
#include "slaanesh/TheDreadPageant.h"
#include "slaanesh/BlissbarbArchers.h"
#include "slaanesh/BlissbarbSeekers.h"
#include "slaanesh/MymideshPainbringers.h"
#include "slaanesh/Shardspeaker.h"
#include "slaanesh/Sigvald.h"
#include "slaanesh/SlaangorFiendbloods.h"
#include "slaanesh/SlickbladeSeekers.h"
#include "slaanesh/SymbareshTwinsouls.h"
#include "slaanesh/GlutosOrscollion.h"
#include "slaanesh/Synessa.h"
#include "slaanesh/Dexcessa.h"

namespace Slaanesh {

    void SlaaneshBase::setHost(Host host) {
        removeKeyword(INVADERS);
        removeKeyword(PRETENDERS);
        removeKeyword(GODSEEKERS);
        removeKeyword(LURID_HAZE_INVADERS_HOST);
        removeKeyword(FAULTLESS_BLADES_PRETENDERS_HOST);
        removeKeyword(SCARLET_CAVALCADE_GODSEEKERS_HOST);

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
            case Host::Lurid_Haze_Invaders:
                addKeyword(LURID_HAZE_INVADERS_HOST);
                addKeyword(INVADERS);
                break;
            case Host::Faultless_Blades_Pretenders:
                addKeyword(FAULTLESS_BLADES_PRETENDERS_HOST);
                addKeyword(PRETENDERS);
                break;
            case Host::Scarlet_Cavalcade_Godseekers:
                addKeyword(SCARLET_CAVALCADE_GODSEEKERS_HOST);
                addKeyword(GODSEEKERS);
                break;
            default:
                break;
        }
    }

    std::string SlaaneshBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Host") {
            auto hostName = magic_enum::enum_name((Host) parameter.intValue);
            return std::string(hostName);
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
        return ParameterValueToString(parameter);
    }

    int SlaaneshBase::EnumStringToInt(const std::string &enumString) {
        auto host = magic_enum::enum_cast<Host>(enumString);
        if (host.has_value()) return (int) host.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

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
            return Rerolls::Ones;
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Territorial) &&
            Board::Instance()->isUnitWithinDeploymentZone(this, owningPlayer())) {
            return Rerolls::Failed;
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Hunter_Supreme) && charged()) {
            return Rerolls::Ones;
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Strongest_Alone)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 6.0);
            if (units.empty()) {
                return Rerolls::Failed;
            }
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

    void SlaaneshBase::onEndCharge(PlayerId player) {
        EventInterface::onEndCharge(player);

        // Locus of Diversion
        if (hasKeyword(HEDONITE) && hasKeyword(DAEMON) && hasKeyword(HERO)) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 1.0)) {
                if (unit->canPileIn()) {
                    auto roll = Dice::RollD6();
                    if (hasKeyword(GREATER_DAEMON)) roll++;
                    if (roll >= 4) {
                        unit->buffMovement(MovementRule::Can_PileIn, false, {GamePhase::Combat, m_battleRound, player});
                    }
                }
            }
        }

        // Maniacal Hunters
        if (isGeneral() && (m_host == Host::Godseekers) && charged()) {
            getRoster()->incrementResource(Dice::RollD3());
            auto units = Board::Instance()->getAllUnits(owningPlayer());
            for (auto unit : units) {
                if (unit->charged()) {
                    getRoster()->incrementResource(1);
                    break;
                }
            }
        }
    }

    Rerolls SlaaneshBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Best_Of_The_Best)) {
            auto heros = Board::Instance()->getUnitsWithKeyword(owningPlayer(), HERO);
            for (auto hero : heros) {
                if ((hero->remainingModels() > 0) && distanceTo(hero) < 6.0) {
                    return Rerolls::Failed;
                }
            }
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Hunter_Supreme) && charged()) {
            return Rerolls::Ones;
        }

        return Unit::toWoundRerolls(weapon, target);
    }

    void SlaaneshBase::onEndCombat(PlayerId player) {

        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Glory_Hog)) {
            auto friendlyUnits = Board::Instance()->getAllUnits(owningPlayer());
            int numUnitsSlain = 0;
            for (auto unit : friendlyUnits) {
                std::function<void(const TurnRecord &)> turnVisitor = [this, &numUnitsSlain](const TurnRecord &record) {
                    if (record.m_round == this->m_battleRound) {
                        numUnitsSlain += record.m_enemyUnitsSlain;
                    }
                };
                unit->getStatistics().visitTurn(turnVisitor);
            }
            if (numUnitsSlain > 0) {
                getRoster()->addCommandPoints(1);
            }
        }
        Unit::onEndCombat(player);
    }

    void SlaaneshBase::onStartCombat(PlayerId player) {
        Unit::onStartHero(player);

        if (isGeneral() && (m_commandTrait == CommandTrait::Hurler_Of_Obscenities)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 6.0);
            if (hero) {
                hero->buffModifier(Attribute::To_Save_Melee, -1, {GamePhase::Combat, m_battleRound, player});
                hero->buffModifier(Attribute::To_Hit_Melee, 1, {GamePhase::Combat, m_battleRound, player});
            }
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Monarch_Of_Lies)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
            if (hero) {
                hero->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Combat, m_battleRound, player});
            }
        }

    }

    int SlaaneshBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Delusions_Of_Infallibility)) {
            mod += 2;
        }
        return mod;
    }

    Wounds SlaaneshBase::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);

        if (isGeneral() && (m_commandTrait == CommandTrait::Hunter_Of_Godbeasts)) {
            damage.normal++;
        }
        return damage;
    }

    Rerolls SlaaneshBase::runRerolls() const {
        // Feverish Anticipation
        if (m_host == Host::Lurid_Haze_Invaders) {
            auto general = getRoster()->getGeneral();
            if (general && distanceTo(general) < 12) {
                auto slaaneshGeneral = dynamic_cast<SlaaneshBase *>(general);
                if (slaaneshGeneral && (slaaneshGeneral->m_commandTrait == CommandTrait::Feverish_Anticipation)) {
                    return Rerolls::Failed;
                }
            }
        }
        return Unit::runRerolls();
    }

    Rerolls SlaaneshBase::battleshockRerolls() const {
        // Embodiment of Hate
        if (m_host == Host::Scarlet_Cavalcade_Godseekers) {
            auto general = getRoster()->getGeneral();
            if (general && distanceTo(general) < 12) {
                auto slaaneshGeneral = dynamic_cast<SlaaneshBase *>(general);
                if (slaaneshGeneral && (slaaneshGeneral->m_commandTrait == CommandTrait::Embodiment_Of_Haste)) {
                    return Rerolls::Failed;
                }
            }
        }

        if (m_host == Host::Pretenders) {
            auto general = getRoster()->getGeneral();
            if (general && distanceTo(general) < 12) {
                auto slaaneshGeneral = dynamic_cast<SlaaneshBase *>(general);
                if (slaaneshGeneral && (slaaneshGeneral->m_commandTrait == CommandTrait::Inspirer)) {
                    return Rerolls::Failed;
                }
            }
        }
        return Unit::battleshockRerolls();
    }

    void SlaaneshBase::onCharged() {
        if (isGeneral() && (m_commandTrait == CommandTrait::Sweeping_Slash)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 2) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                }
            }
        }
        Unit::onCharged();
    }

    void SlaaneshBase::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        if (isGeneral() && (m_commandTrait == CommandTrait::Speed_Chaser)) {
            m_retreatAndCharge = true;
        }
    }

    void SlaaneshBase::onEndBattleshock(PlayerId player) {
        Unit::onEndBattleshock(player);

        // Warlord Supreme
        if (isGeneral() && (m_host == Host::Pretenders)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            if (units.size() >= 2) {
                getRoster()->incrementResource(Dice::RollD3());
            } else if (units.size() == 1) {
                getRoster()->incrementResource(1);
            }
        }

        // Escalating Havoc
        if (isGeneral() && (m_host == Host::Invaders)) {
            if (Board::Instance()->isUnitWithinDeploymentZone(this, GetEnemyId(owningPlayer()))) {
                getRoster()->incrementResource(1);
            }
            // TODO: Handle the case if the roster has more than one general.
        }
    }

    void
    SlaaneshBase::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon, const Wounds &weaponDamage) {
        Unit::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        if (isGeneral() && (m_commandTrait == CommandTrait::Skin_Taker) && (numSlain > 0) && weapon->isMelee()) {
            heal(Dice::RollD3());
        }
    }

    int SlaaneshBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Send Me Your Best
        if ((m_host == Host::Faultless_Blades_Pretenders) && target->hasKeyword(HERO) && charged()) {
            mod++;
        }
        return mod;
    }

    void Init() {
        Daemonettes::Init();
        Fiends::Init();
        Seekers::Init();
        KeeperOfSecrets::Init();
        HellstridersWithClawspears::Init();
        HellstridersWithHellscourges::Init();
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
        TheDreadPageant::Init();
        BlissbarbArchers::Init();
        BlissbarbSeekers::Init();
        MyrmideshPainbringers::Init();
        ShardspeakerOfSlaanesh::Init();
        SigvaldPrinceOfSlaanesh::Init();
        SlaangorFiendbloods::Init();
        SlickbladeSeekers::Init();
        SymbareshTwinsouls::Init();
        GlutosOrscollion::Init();
        Synessa::Init();
        Dexcessa::Init();
    }

} // namespace Slaanesh
