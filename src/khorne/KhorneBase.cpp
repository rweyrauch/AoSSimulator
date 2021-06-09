/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/KhorneBase.h>
#include <Board.h>
#include <Roster.h>
#include <magic_enum.hpp>

#include "khorne/AspiringDeathbringer.h"
#include "khorne/BloodThrone.h"
#include "khorne/BloodWarriors.h"
#include "khorne/Bloodcrushers.h"
#include "khorne/Bloodletters.h"
#include "khorne/Bloodmaster.h"
#include "khorne/Bloodreavers.h"
#include "khorne/Bloodsecrator.h"
#include "khorne/Bloodstoker.h"
#include "khorne/BloodthirsterOfInsensateRage.h"
#include "khorne/BloodthirsterOfUnfetteredFury.h"
#include "khorne/ExaltedDeathbringer.h"
#include "khorne/ExaltedGreaterDaemon.h"
#include "khorne/FleshHounds.h"
#include "khorne/GarreksReavers.h"
#include "khorne/Karanak.h"
#include "khorne/Khorgoraths.h"
#include "khorne/KorghosKhul.h"
#include "khorne/LordOfKhorneOnJuggernaut.h"
#include "khorne/MagoresFiends.h"
#include "khorne/MazarallTheButcher.h"
#include "khorne/MightyLordOfKhorne.h"
#include "khorne/MightySkullcrushers.h"
#include "khorne/Riptooth.h"
#include "khorne/ScylaAnfingrimm.h"
#include "khorne/SkaaracTheBloodborn.h"
#include "khorne/Skarbrand.h"
#include "khorne/SkarrBloodwrath.h"
#include "khorne/SkullCannons.h"
#include "khorne/Skullgrinder.h"
#include "khorne/Skullmaster.h"
#include "khorne/Skullreapers.h"
#include "khorne/Skulltaker.h"
#include "khorne/Slaughterpriest.h"
#include "khorne/ValkiaTheBloody.h"
#include "khorne/VorgarothAndSkalok.h"
#include "khorne/WrathOfKhorneBloodthirster.h"
#include "khorne/Wrathmongers.h"

namespace Khorne {

    bool KhorneBase::s_claimedBloodTithe = false;
    BloodTitheReward KhorneBase::s_currentBloodTithe = BloodTitheReward::None;
    bool KhorneBase::s_slaughterTriumphantActive = false;
    bool KhorneBase::s_crimsonRainActive = false;

    void KhorneBase::setSlaughterHost(SlaughterHost host) {
        removeKeyword(REAPERS_OF_VENGEANCE);
        removeKeyword(BLOODLORDS);
        removeKeyword(GORETIDE);
        removeKeyword(SKULLFIEND_TRIBE);

        m_slaughterHost = host;
        switch (m_slaughterHost) {
            case SlaughterHost::Reapers_Of_Vengeance:
                s_globalBattleshockFleeModifier.connect(this, &KhorneBase::devourTheCraven, &m_devoutTheCravenSlot);
                addKeyword(REAPERS_OF_VENGEANCE);
                break;
            case SlaughterHost::Bloodlords:
                addKeyword(BLOODLORDS);
                break;
            case SlaughterHost::Goretide:
                addKeyword(GORETIDE);
                break;
            case SlaughterHost::Skullfiend_Tribe:
                addKeyword(SKULLFIEND_TRIBE);
                break;
            default:
                break;
        }
    }

    Rerolls KhorneBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Slay the Mighty
        if (m_slaughterHost == SlaughterHost::Bloodlords) {
            if (hasKeyword(DAEMON) && (target->hasKeyword(HERO) || target->hasKeyword(MONSTER))) {
                return Rerolls::Ones;
            }
        }
            // Tireless Conquerors
        else if (m_slaughterHost == SlaughterHost::Goretide) {
            if (hasKeyword(MORTAL)) {
                auto numObjMarkers = Board::Instance()->getNumObjectives();
                for (auto i = 0; i < numObjMarkers; i++) {
                    const auto obj = Board::Instance()->getObjective(i);
                    if (obj) {
                        if (position().distance(obj->m_pos) <= 12.0) {
                            return Rerolls::Ones;
                        }
                    }
                }
            }
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Hungry_For_Glory)) {
            if (target->hasKeyword(MONSTER) || target->hasKeyword(HERO)) {
                return Rerolls::Failed;
            }
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    Rerolls KhorneBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Skull Hunters
        if (m_slaughterHost == SlaughterHost::Skullfiend_Tribe) {
            if (hasKeyword(MORTAL)) {
                auto hero = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 12.0);
                if (hero) {
                    return Rerolls::Ones;
                }
            }
        }

        // Locus of Fury
        if (hasKeyword(DAEMON)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 16.0);
            for (auto ip : units) {
                if (ip->hasKeyword(GREATER_DAEMON)) {
                    return Rerolls::Ones;
                }
                if (ip->hasKeyword(DAEMON) && distanceTo(ip) <= 12.0) {
                    return Rerolls::Ones;
                }
            }
        }

        if (isGeneral() && (weapon->isMelee() && (m_commandTrait == CommandTrait::Slaughterborn))) {
            return Rerolls::Failed;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Hungry_For_Glory)) {
            if (target->hasKeyword(MONSTER) || target->hasKeyword(HERO)) {
                return Rerolls::Failed;
            }
        }

        return Unit::toHitRerolls(weapon, target);
    }

    std::string KhorneBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Slaughter Host") {
            auto hostName = magic_enum::enum_name((SlaughterHost) parameter.intValue);
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
        return ParameterValueToString(parameter);
    }

    int KhorneBase::EnumStringToInt(const std::string &enumString) {
        auto host = magic_enum::enum_cast<SlaughterHost>(enumString);
        if (host.has_value()) return (int) host.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        return 0;
    }

    void KhorneBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;

        // Mage Eater
        m_totalUnbinds++;

        if (m_commandTrait == CommandTrait::Aspect_Of_Death) {
            s_globalBattleshockFleeModifier.connect(this, &KhorneBase::aspectOfDeath, &m_aspectOfDeathSlot);
        }
    }

    void KhorneBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void KhorneBase::onFriendlyUnitSlain(const Unit *attacker) {
        Unit::onFriendlyUnitSlain(nullptr);

        // Add a Blood Tithe point
        getRoster()->incrementResource(1);
    }

    void KhorneBase::onEnemyUnitSlain(const Unit *unit) {
        Unit::onEnemyUnitSlain(unit);

        // Add a Blood Tithe point
        getRoster()->incrementResource(1);
    }

    void KhorneBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Blood Tithe check
        if (!s_claimedBloodTithe && (getRoster()->getPoints() > 0)) {

            // TODO: Determine what/if to use a reward
            s_claimedBloodTithe = selectBloodTitheReward(s_currentBloodTithe);
            if (s_claimedBloodTithe) {
                switch (s_currentBloodTithe) {
                    case BloodTitheReward::None:
                        break;
                    case BloodTitheReward::Bloody_Examplar:
                        getRoster()->addCommandPoints(1);
                        break;
                    case BloodTitheReward::Spelleater_Curse:
                        break;
                    case BloodTitheReward::Murderlust:
                        break;
                    case BloodTitheReward::Apoplectic_Frenzy:
                        break;
                    case BloodTitheReward::Brass_Skull_Meteor:
                        dropMeteor();
                        break;
                    case BloodTitheReward::Relentless_Fury:
                        // Nothing else to do - the current tithe is set to Relentless_Fury
                        break;
                    case BloodTitheReward::Crimson_Rain:
                        s_crimsonRainActive = true;
                        break;
                    case BloodTitheReward::Slaughter_Triumphant: {
                        auto khorneUnits = Board::Instance()->getAllUnits(owningPlayer());
                        for (auto unit : khorneUnits) {
                            if (unit->hasKeyword(KHORNE)) {
                                unit->buffAbility(Ability::Extra_Hit_On_Value, 6,
                                                  {GamePhase::Hero, DurationRestOfGame, owningPlayer()});
                            }
                        }
                        s_slaughterTriumphantActive = true;
                    }
                        break;
                    default:
                        break;
                }
            }

            // Bloody Exemplar

            if (s_claimedBloodTithe) {
                PLOG_INFO << "KHORE:  Claming Blood Tithe reward " << magic_enum::enum_name(s_currentBloodTithe);
                getRoster()->clearAvailableResource();
            }
        }

        if (s_crimsonRainActive) {
            if (hasKeyword(KHORNE)) {
                heal(Dice::RollD3());
            }
        }
    }

    void KhorneBase::onRestore() {
        Unit::onRestore();

        s_claimedBloodTithe = false;
        s_currentBloodTithe = BloodTitheReward::None;
        s_slaughterTriumphantActive = false;
        s_crimsonRainActive = false;
    }

    void KhorneBase::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        Unit::onFriendlyModelSlain(numSlain, attacker, source);

        // Relentless Fury
        if (s_currentBloodTithe == BloodTitheReward::Relentless_Fury) {
            int numEnemySlain = 0;
            if (meleeTarget() != nullptr) {
                doPileIn();
                fight(numSlain, meleeTarget(), numEnemySlain);
            }
        }
    }

    void KhorneBase::onBeginTurn(int battleRound, PlayerId player) {
        Unit::onBeginTurn(battleRound, player);

        // BeginTurn happens before any the hero phase starts - can safely clear the static flags here
        s_currentBloodTithe = BloodTitheReward::None;
        s_claimedBloodTithe = false;
    }

    bool KhorneBase::selectBloodTitheReward(BloodTitheReward &selectedReward) {
        selectedReward = BloodTitheReward::None;

        if ((getRoster()->getCommandPoints() <= 1) && (getRoster()->getPoints() == 1)) {
            getRoster()->addCommandPoints(1);
            selectedReward = BloodTitheReward::Bloody_Examplar;
            return true;
        }

        // Slaughter Triumphant
        if (!s_slaughterTriumphantActive && (getRoster()->getPoints() >= 8)) {
            selectedReward = BloodTitheReward::Slaughter_Triumphant;
            return true;
        }

        // Crimson Rain
        if (!s_crimsonRainActive && (getRoster()->getPoints() >= 7)) {
            selectedReward = BloodTitheReward::Crimson_Rain;
            return true;
        }

        // Relentless Fury
        if (getRoster()->getPoints() >= 6) {
            selectedReward = BloodTitheReward::Relentless_Fury;
            return true;
        }

        // Brass Skull Meteor
        if (getRoster()->getPoints() >= 5) {
            selectedReward = BloodTitheReward::Brass_Skull_Meteor;
            return true;
        }

        return false;
    }

    void KhorneBase::dropMeteor() {
        auto units = Board::Instance()->getAllUnits(GetEnemyId(owningPlayer()));
        Unit *bestTarget = nullptr;
        int bestUnitCount = 0;
        for (auto unit : units) {
            auto closestUnits = Board::Instance()->getUnitsWithin(unit, unit->owningPlayer(), 8.0);
            if ((int) closestUnits.size() > bestUnitCount) {
                bestTarget = unit;
                bestUnitCount = (int) closestUnits.size();
            }
        }
        if (bestTarget) {
            if (Dice::RollD6() >= 3) {
                bestTarget->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability, nullptr}, this);
            }
            auto enemies = Board::Instance()->getUnitsWithin(bestTarget, bestTarget->owningPlayer(), 8.0);
            for (auto enemy : enemies) {
                if (Dice::RollD6() == 6) {
                    enemy->applyDamage({0, 1, Wounds::Source::Ability, nullptr}, this);
                }
            }
        }
    }

    void KhorneBase::onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) {
        Unit::onEnemyModelSlain(numSlain, enemyUnit, source);

        if (isGeneral() && (m_commandTrait == CommandTrait::Arch_Slaughterer)) {
            if ((source == Wounds::Source::Weapon_Melee) &&
                ((enemyUnit->hasKeyword(MONSTER)) || (enemyUnit->hasKeyword(HERO)))) {
                // Get an extra Blood Tithe point for MONSTERS and HEROS.
                getRoster()->incrementResource(1);
            }
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Master_Decapitator)) {
            if (enemyUnit->hasKeyword(HERO)) {
                // Get an extra Blood Tithe point for HEROS.
                getRoster()->incrementResource(1);
            }
        }
    }

    Rerolls KhorneBase::chargeRerolls() const {
        auto general = getRoster()->getGeneral();
        if (general && (general->remainingModels() > 0) && (distanceTo(general) < 12.0)) {
            auto khorne = dynamic_cast<KhorneBase *>(general);
            if (khorne && (khorne->m_commandTrait == CommandTrait::Violent_Urgency)) {
                return Rerolls::Failed;
            }
        }
        if (isGeneral() && hasKeyword(BLOODLORDS) && (m_commandTrait == CommandTrait::Slaughterers_Thirst)) {
            return Rerolls::Failed;
        }
        return Unit::chargeRerolls();
    }

    Wounds KhorneBase::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;
        if (isGeneral() && (m_commandTrait == CommandTrait::Berserker_Lord)) {
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 5, numSixes);
        }
        return Unit::applyWoundSave(totalWounds, attackingUnit);
    }

    void KhorneBase::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        if (isGeneral() && (m_commandTrait == CommandTrait::Mark_Of_The_Cannibal) &&
            (m_currentRecord.m_enemyModelsSlain > 0)) {
            heal(1);
        }
    }

    int KhorneBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);

        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Disciple_Of_Khorne)) {
            attacks += 2;
        }
        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Rage_Unchained)) {
            attacks += 1;
        }

        return attacks;
    }

    Rerolls KhorneBase::prayerRerolls() const {
        if (hasKeyword(FLAYED) && hasKeyword(PRIEST)) {
            auto general = getRoster()->getGeneral();
            if (general && (general->remainingModels() > 0) && (distanceTo(general) < 8.0)) {
                auto khorne = dynamic_cast<KhorneBase *>(general);
                if (khorne && (khorne->m_commandTrait == CommandTrait::Vessel_Of_Butchery)) {
                    return Rerolls::Ones;
                }
            }
        }
        return Unit::prayerRerolls();
    }

    int KhorneBase::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (hasKeyword(BALEFUL_LORDS) && hasKeyword(BLOODTHIRSTER)) {
            auto general = getRoster()->getGeneral();
            if (general && (general->remainingModels() > 0) && (distanceTo(general) < 8.0)) {
                auto khorne = dynamic_cast<KhorneBase *>(general);
                if (khorne && (khorne->m_commandTrait == CommandTrait::Thirst_For_Carnage)) {
                    mod++;
                }
            }
        }
        return mod;
    }

    int KhorneBase::moveModifier() const {
        auto mod = Unit::moveModifier();

        if (isGeneral() && hasKeyword(BLOODLORDS) && (m_commandTrait == CommandTrait::Slaughterers_Thirst)) {
            mod += 4;
        }
        return mod;
    }

    Wounds KhorneBase::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);

        if (isGeneral() && weapon->isMelee() && hasKeyword(GORETIDE) && (m_commandTrait == CommandTrait::Hew_The_Foe)) {
            damage.normal++;
        }
        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Devastating_Blow) &&
            (woundRoll == 6)) {
            damage.normal = 0;
            damage.mortal = weapon->damage();
        }
        return damage;
    }

    int KhorneBase::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        auto general = dynamic_cast<KhorneBase *>(getRoster()->getGeneral());
        if (general && (general->remainingModels() > 0) && (general->m_commandTrait == CommandTrait::Bloodsworn) &&
            (distanceTo(general) < 16.0)) {
            mod++;
        }
        return mod;
    }

    KhorneBase::~KhorneBase() {
        m_devoutTheCravenSlot.disconnect();
        m_aspectOfDeathSlot.disconnect();
    }

    int KhorneBase::devourTheCraven(const Unit *unit, int roll) {
        if (unit && (distanceTo(unit) < 3.0)) {
            return Dice::RollD3();
        }
        return 0;
    }

    int KhorneBase::aspectOfDeath(const Unit *unit, int roll) {
        if (isGeneral() && (m_commandTrait == CommandTrait::Aspect_Of_Death) && unit && (distanceTo(unit) < 8.0)) {
            return Dice::RollD3();
        }
        return 0;
    }

    void Init() {
        AspiringDeathbringer::Init();
        BloodWarriors::Init();
        Bloodcrushers::Init();
        Bloodletters::Init();
        Bloodmaster::Init();
        Bloodreavers::Init();
        Bloodsecrator::Init();
        Bloodstoker::Init();
        BloodthirsterOfInsensateRage::Init();
        BloodthirsterOfUnfetteredFury::Init();
        ExaltedDeathbringer::Init();
        ExaltedGreaterDaemonOfKhorne::Init();
        FleshHounds::Init();
        GarreksReavers::Init();
        HeraldOfKhorneOnBloodThrone::Init();
        Karanak::Init();
        Khorgoraths::Init();
        KorghosKhul::Init();
        LordOfKhorneOnJuggernaut::Init();
        MagoresFiends::Init();
        MazarallTheButcher::Init();
        MightyLordOfKhorne::Init();
        MightySkullcrushers::Init();
        Riptooth::Init();
        ScylaAnfingrimm::Init();
        SkaaracTheBloodborn::Init();
        Skarbrand::Init();
        SkarrBloodwrath::Init();
        SkullCannons::Init();
        Skullgrinder::Init();
        Skullmaster::Init();
        Skullreapers::Init();
        Skulltaker::Init();
        Slaughterpriest::Init();
        ValkiaTheBloody::Init();
        VorgarothAndSkalok::Init();
        WrathOfKhorneBloodthirster::Init();
        Wrathmongers::Init();
    }

} //namespace Khorne