/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/SoulblightGravelords.h>
#include <Board.h>
#include <Roster.h>
#include <magic_enum.hpp>
#include <spells/MysticShield.h>
#include <soulblight/BelladammaVolga.h>
#include <soulblight/Gorslav.h>
#include <soulblight/KosargiNightguard.h>
#include <soulblight/Kritza.h>
#include <soulblight/LaukaVai.h>
#include <soulblight/LadyAnnika.h>
#include <soulblight/PrinceDuvalle.h>
#include <soulblight/RadukarTheBeast.h>
#include <soulblight/RadukarTheWolf.h>
#include <soulblight/Terrorgheist.h>
#include <soulblight/TheCrimsonCourt.h>
#include <soulblight/Torgillius.h>
#include <soulblight/Vargskyr.h>
#include <soulblight/VengorianLord.h>
#include <soulblight/VyrkosBloodborn.h>
#include <soulblight/WatchCaptainHalgrim.h>
#include <soulblight/WightKingOnSteed.h>
#include <soulblight/ZombieDragon.h>

#include "soulblight/Nagash.h"
#include "soulblight/BlackKnights.h"
#include "soulblight/BloodKnights.h"
#include "soulblight/DireWolves.h"
#include "soulblight/GraveGuard.h"
#include "soulblight/DeathrattleSkeletons.h"
#include "soulblight/Vargheists.h"
#include "soulblight/Zombies.h"
#include "soulblight/WightKing.h"
#include "soulblight/Necromancer.h"
#include "soulblight/BloodseekerPalangquin.h"
#include "soulblight/CorpseCartBrazier.h"
#include "soulblight/CorpseCartLodestone.h"
#include "soulblight/CovenThrone.h"
#include "soulblight/FellBats.h"
#include "soulblight/Mannfred.h"
#include "soulblight/MortisEngine.h"
#include "soulblight/Neferata.h"
#include "soulblight/PrinceVhordrai.h"
#include "soulblight/VampireLord.h"
#include "soulblight/VampireLordOnDragon.h"

namespace Soulblight {

    std::string SoulblightBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Bloodline") {
            auto bloodlineName = magic_enum::enum_name((CursedBloodline) parameter.intValue);
            return std::string(bloodlineName);
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

    int SoulblightBase::EnumStringToInt(const std::string &enumString) {
        auto bloodline = magic_enum::enum_cast<CursedBloodline>(enumString);
        if (bloodline.has_value()) return (int) bloodline.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        return 0;
    }

    SoulblightBase::~SoulblightBase() {
        m_terrifyVisageSlot.disconnect();
    }

    void SoulblightBase::setBloodline(CursedBloodline bloodline) {
        removeKeyword(LEGION_OF_NIGHT);
        removeKeyword(LEGION_OF_BLOOD);

        m_bloodline = bloodline;
        switch (bloodline) {
            case CursedBloodline::Legion_Of_Night:
                addKeyword(LEGION_OF_NIGHT);
                break;
            case CursedBloodline::Legion_Of_Blood:
                addKeyword(LEGION_OF_BLOOD);
                break;

            default:
                break;
        }
    }

    void SoulblightBase::deathlyInvocations(int numUnits, double range) {

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

    void SoulblightBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;

        if (m_commandTrait == CommandTrait::Terrifying_Visage) {
            s_globalBraveryMod.connect(this, &SoulblightBase::terrifyingVisage, &m_terrifyVisageSlot);
        }
        else {
            m_terrifyVisageSlot.disconnect();
        }

    }

    void SoulblightBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    Wounds
    SoulblightBase::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        if (m_hasBloodiedStrength) {
            if (!weapon->isMount() && weapon->isMelee()) {
                damage.normal++;
            }
        }
        return damage;
    }

    Rerolls SoulblightBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Merciless_Hunter)) {
            return Rerolls::Ones;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int SoulblightBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        return attacks;
    }

    Rerolls SoulblightBase::chargeRerolls() const {
        auto general = dynamic_cast<SoulblightBase *>(getRoster()->getGeneral());
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

    int SoulblightBase::runModifier() const {
        auto mod = Unit::runModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Swift_Form)) {
            mod += 2;
        }
        if (m_hasAbsorbedSpeed) {
            mod += 2;
        }
        return mod;
    }

    int SoulblightBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (m_hasStolenVitality) {
            mod++;
        }
        return mod;
    }

    Rerolls SoulblightBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        return Unit::toHitRerolls(weapon, target);
    }

    int SoulblightBase::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        if (isGeneral() && (m_commandTrait == CommandTrait::Aura_Of_Dark_Majesty) && weapon->isMelee()) {
            mod--;
        }
        return mod;
    }

    int SoulblightBase::moveModifier() const {
        auto mod = Unit::moveModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Sanguine_Blur)) {
            mod += 2;
        }
        return mod;
    }

    Rerolls SoulblightBase::battleshockRerolls() const {
        auto general = dynamic_cast<SoulblightBase *>(getRoster()->getGeneral());
        if (general && (general->remainingModels() > 0) && (general->m_commandTrait == CommandTrait::Unbending_Will) &&
            hasKeyword(LEGION_OF_NIGHT) && (distanceTo(general) < 12.0)) {
            return Rerolls::Failed;
        }
        return Unit::battleshockRerolls();
    }

    void SoulblightBase::onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) {
        Unit::onEnemyModelSlain(numSlain, enemyUnit, source);

        if (isGeneral() && (m_commandTrait == CommandTrait::Soul_Crushing_Contempt)) {
            enemyUnit->buffModifier(Attribute::Bravery, -1, {GamePhase::Battleshock, m_battleRound, owningPlayer()});
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Unholy_Impetus)) {
            auto unit = Board::Instance()->getNearestUnit(this, owningPlayer());
            if (unit && (distanceTo(unit) < 3.0)) {
                unit->buffModifier(Attribute::Attacks_Melee, 1, {GamePhase::Combat, m_battleRound, owningPlayer()});
            }
        }
    }

    int SoulblightBase::terrifyingVisage(const Unit *unit) {
        // Slot only connected when trait selected
        if (distanceTo(unit) < 6.0) {
            return -1;
        }
        return 0;
    }

    Wounds SoulblightBase::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Deathless Minions
        if (hasKeyword(SOULBLIGHT_GRAVELORDS)) {
            auto heroes = Board::Instance()->getUnitsWithKeyword(owningPlayer(), HERO);
            for (auto hero : heroes) {
                if ((hero->remainingModels() > 0) && hero->hasKeyword(SOULBLIGHT_GRAVELORDS) && (distanceTo(hero) < 12)) {
                    return ignoreWounds(wounds, m_deathlessMinionsThreshold);
                }
            }

            // TODO: also check for gravesites
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    void SoulblightBase::onEnemyUnitSlain(const Unit *enemyUnit) {
        EventInterface::onEnemyUnitSlain(enemyUnit);

        // Might of the Crimson Keep
        if (hasKeyword(KASTELAI_DYNASTY) && hasKeyword(VAMPIRE)) {
            if (enemyUnit->hasKeyword(HERO) || enemyUnit->hasKeyword(MONSTER)) {
                // Bloodied Strength
                m_hasBloodiedStrength = true;
            }
            else if (enemyUnit->wounds() >= 3) {
                // Stolen Vitality
                m_hasStolenVitality = true;
            }
            else { // (enemyUnit->wounds() <= 2)
                // Absorbed Speed
                m_hasAbsorbedSpeed = true;
            }
        }
    }

    void SoulblightBase::onRestore() {
        EventInterface::onRestore();

        m_hasBloodiedStrength = false;
        m_hasStolenVitality = false;
        m_hasAbsorbedSpeed = false;
    }

    int SoulblightBase::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hasAbsorbedSpeed) {
            mod += 2;
        }
        return mod;
    }

    int SoulblightBase::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);
        // The Bait
        if ((m_battleRound == 1) && hasKeyword(LEGION_OF_NIGHT) && (hasKeyword(DEATHRATTLE) || hasKeyword(DEADWALKERS))) {
            mod++;
        }
        return mod;
    }

    Rerolls SoulblightBase::castingRerolls() const {
        // The Stength of the Wolf is the Pack
        if (hasKeyword(VYRKOS_DYNASTY) && hasKeyword(VAMPIRE)) {
            return Rerolls::Failed;
        }
        return UnitModifierInterface::castingRerolls();
    }

    int SoulblightBase::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        // The Strength of the Pack is the Wolf
        if (weapon->isMelee() && hasKeyword(VYRKOS_DYNASTY) && (hasKeyword(DEATHRATTLE) || hasKeyword(DEADWALKERS))) {
            auto heroes = Board::Instance()->getUnitsWithKeyword(owningPlayer(), HERO);
            for (auto hero : heroes) {
                if ((hero->remainingModels() > 0) && hero->hasKeyword(VYRKOS_DYNASTY) && hero->hasKeyword(VAMPIRE) && (distanceTo(hero) < 9.0)) {
                    mod++;
                }
            }
        }
        return mod;
    }

    int SoulblightBase::targetWoundModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetWoundModifier(weapon, attacker);
        // Monstrous Might
        if (weapon->isMelee()) {
            if (hasKeyword(AVENGORII_DYNASTY) &&
                (hasKeyword(TERRORGHEIST) ||
                 hasKeyword(ZOMBIE_DRAGON) ||
                 (hasKeyword(VAMPIRE) && hasKeyword(MONSTER)))) {
                if (!attacker->hasKeyword(MONSTER)) {
                    mod--;
                }
            }
        }
        return mod;
    }

    void Init() {
        BelladammaVolga::Init();
        BlackKnights::Init();
        BloodKnights::Init();
        BloodseekerPalanquin::Init();
        CorpseCartWithBalefireBrazier::Init();
        CorpseCartWithUnholyLodestone::Init();
        CovenThrone::Init();
        DeathrattleSkeletons::Init();
        DireWolves::Init();
        FellBats::Init();
        GorslavTheGravekeeper::Init();
        GraveGuard::Init();
        KosargiNightguard::Init();
        KritzaTheRatPrince::Init();
        LadyAnnika::Init();
        LaukaVai::Init();
        MannfredMortarchOfNight::Init();
        MortisEngine::Init();
        Nagash::Init();
        Necromancer::Init();
        NeferataMortarchOfBlood::Init();
        PrinceDuvalle::Init();
        PrinceVhordrai::Init();
        RadukarTheBeast::Init();
        RadukarTheWolf::Init();
        Terrorgheist::Init();
        TheCrimsonCourt::Init();
        TorgilliusTheChamberlain::Init();
        VampireLord::Init();
        VampireLordOnZombieDragon::Init();
        Vargheists::Init();
        Vargskyr::Init();
        VengorianLord::Init();
        VyrkosBloodborn::Init();
        WatchCaptainHalgrim::Init();
        WightKing::Init();
        WightKingOnSteed::Init();
        ZombieDragon::Init();
        Zombies::Init();
    }

} //namespace Seraphon
