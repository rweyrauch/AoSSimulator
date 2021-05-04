/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/LegionOfNagash.h>
#include <Board.h>
#include <Roster.h>
#include <magic_enum.hpp>

#include "death/Nagash.h"
#include "death/BlackKnights.h"
#include "death/BloodKnights.h"
#include "death/DireWolves.h"
#include "death/GraveGuard.h"
#include "death/SkeletonWarriors.h"
#include "death/Vargheists.h"
#include "death/Zombies.h"
#include "death/WightKingTombBlade.h"
#include "death/WightKingBlackAxe.h"
#include "death/Necromancer.h"
#include "death/BatSwarms.h"
#include "death/BloodseekerPalangquin.h"
#include "death/CorpseCartBrazier.h"
#include "death/CorpseCartLodestone.h"
#include "death/CovenThrone.h"
#include "death/FellBats.h"
#include "death/Mannfred.h"
#include "death/MortisEngine.h"
#include "death/Neferata.h"
#include "death/PrinceVhordrai.h"
#include "death/VampireLord.h"
#include "death/VampireLordOnDragon.h"

namespace Death {

    std::string LegionOfNagashBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Legion") {
            auto legionName = magic_enum::enum_name((Legion) parameter.intValue);
            return std::string(legionName);
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

    int LegionOfNagashBase::EnumStringToInt(const std::string &enumString) {
        auto legion = magic_enum::enum_cast<Legion>(enumString);
        if (legion.has_value()) return (int) legion.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        return 0;
    }

    void LegionOfNagashBase::setLegion(Legion legion) {
        removeKeyword(GRAND_HOST_OF_NAGASH);
        removeKeyword(LEGION_OF_SACRAMENT);
        removeKeyword(LEGION_OF_NIGHT);
        removeKeyword(LEGION_OF_BLOOD);

        m_legion = legion;
        switch (legion) {
            case Legion::Grand_Host_Of_Nagash:
                addKeyword(GRAND_HOST_OF_NAGASH);
                break;
            case Legion::Legion_Of_Sacrament:
                addKeyword(LEGION_OF_SACRAMENT);
                break;
            case Legion::Legion_Of_Night:
                addKeyword(LEGION_OF_NIGHT);
                break;
            case Legion::Legion_Of_Blood:
                addKeyword(LEGION_OF_BLOOD);
                break;
            default:
                break;
        }
    }

    void LegionOfNagashBase::deathlyInvocations(int numUnits, double range) {

        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), range);
        int unitsHealed = 0;
        for (auto unit : units) {
            if (unit->hasKeyword(SUMMONABLE) && (unit->remainingWounds() < unit->initialWounds())) {
                const auto woundsRestored = Dice::RollD3();
                if (unit->numOfWoundedModels() > 0) {
                    unit->heal(woundsRestored);
                } else {
                    const auto numModels = woundsRestored / unit->wounds();
                    if (numModels)
                        unit->returnModels(numModels);
                }
                unitsHealed++;
            }

            if (unitsHealed >= numUnits) break;
        }
    }

    void LegionOfNagashBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void LegionOfNagashBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    Wounds
    LegionOfNagashBase::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        if (isGeneral() && (m_commandTrait == CommandTrait::Chosen_Champion) && weapon->isMelee() &&
            target->hasKeyword(HERO)) {
            damage.normal++;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Killing_Blow) && weapon->isMelee() && (woundRoll == 6)) {
            damage.mortal++;
        }

        return damage;
    }

    Rerolls LegionOfNagashBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Bane_Of_The_Living) && !target->hasKeyword(DEATH)) {
            return Rerolls::Ones;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Merciless_Hunter)) {
            return Rerolls::Ones;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int LegionOfNagashBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        auto general = dynamic_cast<LegionOfNagashBase *>(getRoster()->getGeneral());
        if (general && (general->remainingModels() > 0) &&
            (general->m_commandTrait == CommandTrait::Lord_Of_Nagashizzar) && hasKeyword(DEATHRATTLE) &&
            (distanceTo(general) < 6.0)) {
            attacks++;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Blood_Fury) && weapon->isMelee()) {
            attacks++;
        }
        return attacks;
    }

    Rerolls LegionOfNagashBase::chargeRerolls() const {
        auto general = dynamic_cast<LegionOfNagashBase *>(getRoster()->getGeneral());
        if (general && (general->remainingModels() > 0) &&
            (general->m_commandTrait == CommandTrait::Ancient_Strategist) && hasKeyword(DEATHRATTLE) &&
            (distanceTo(general) < 9.0)) {
            return Rerolls::Failed;
        }
        if (general && (general->remainingModels() > 0) &&
            (general->m_commandTrait == CommandTrait::Emissary_Of_The_Master) && hasKeyword(DEATH) &&
            (distanceTo(general) < 6.0)) {
            return Rerolls::Failed;
        }
        if (general && (general->remainingModels() > 0) &&
            (general->m_commandTrait == CommandTrait::Aristocracy_Of_Blood) && hasKeyword(SOULBLIGHT) &&
            (distanceTo(general) < 9.0)) {
            return Rerolls::Failed;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Sanguine_Blur)) {
            return Rerolls::Failed;
        }
        return Unit::chargeRerolls();
    }

    void LegionOfNagashBase::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (isGeneral() && (m_commandTrait == CommandTrait::Aura_Of_Ages)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 4) {
                    unit->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Combat, m_battleRound, owningPlayer()});
                }
            }
        }
    }

    int LegionOfNagashBase::runModifier() const {
        auto mod = Unit::runModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Swift_Form)) {
            mod += 2;
        }
        return mod;
    }

    int LegionOfNagashBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Curse_Of_The_Revenant)) {
            mod++;
        }
        return mod;
    }

    Rerolls LegionOfNagashBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Deathless_Duellist) && target->hasKeyword(HERO)) {
            return Rerolls::Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int LegionOfNagashBase::castingModifier() const {
        auto mod = Unit::castingModifier();
        if (isGeneral() && hasKeyword(WIZARD) && (m_commandTrait == CommandTrait::Red_Fury)) {
            mod++;
        }
        return mod;
    }

    int LegionOfNagashBase::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();
        if (isGeneral() && hasKeyword(WIZARD) && (m_commandTrait == CommandTrait::Red_Fury)) {
            mod++;
        }
        return mod;
    }

    int LegionOfNagashBase::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        auto hits = Unit::generateHits(unmodifiedHitRoll, weapon, unit);
        if ((unmodifiedHitRoll == 6) && isGeneral() && weapon->isMelee() &&
            (m_commandTrait == CommandTrait::Swift_Strikes)) {
            hits++;
        }
        return hits;
    }

    int LegionOfNagashBase::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        if (isGeneral() && (m_commandTrait == CommandTrait::Aura_Of_Dark_Majesty) && weapon->isMelee()) {
            mod--;
        }
        return mod;
    }

    int LegionOfNagashBase::moveModifier() const {
        auto mod = Unit::moveModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Sanguine_Blur)) {
            mod += 2;
        }
        return mod;
    }

    Rerolls LegionOfNagashBase::battleshockRerolls() const {
        auto general = dynamic_cast<LegionOfNagashBase *>(getRoster()->getGeneral());
        if (general && (general->remainingModels() > 0) && (general->m_commandTrait == CommandTrait::Unbending_Will) &&
            hasKeyword(LEGION_OF_NIGHT) && (distanceTo(general) < 12.0)) {
            return Rerolls::Failed;
        }
        return Unit::battleshockRerolls();
    }

    void Init() {
        Nagash::Init();
        BlackKnights::Init();
        BloodKnights::Init();
        DireWolves::Init();
        GraveGuard::Init();
        SkeletonWarriors::Init();
        Vargheists::Init();
        Zombies::Init();
        WightKingWithBlackAxe::Init();
        WightKingWithBalefulTombBlade::Init();
        Necromancer::Init();
        BatSwarms::Init();
        BloodseekerPalanquin::Init();
        CorpseCartWithBalefireBrazier::Init();
        CorpseCartWithUnholyLodestone::Init();
        CovenThrone::Init();
        FellBats::Init();
        MannfredMortarchOfNight::Init();
        MortisEngine::Init();
        NeferataMortarchOfBlood::Init();
        PrinceVhordrai::Init();
        VampireLord::Init();
        VampireLordOnZombieDragon::Init();
    }

} //namespace Seraphon
