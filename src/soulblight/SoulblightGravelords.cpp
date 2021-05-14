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
#include "soulblight/SkeletonWarriors.h"
#include "soulblight/Vargheists.h"
#include "soulblight/Zombies.h"
#include "soulblight/WightKingBlackAxe.h"
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
#include "Lore.h"

namespace Soulblight {

    std::string SoulblightBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Legion") {
            auto legionName = magic_enum::enum_name((CursedBloodline) parameter.intValue);
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

    int SoulblightBase::EnumStringToInt(const std::string &enumString) {
        auto legion = magic_enum::enum_cast<CursedBloodline>(enumString);
        if (legion.has_value()) return (int) legion.value();

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

    void SoulblightBase::setBloodline(CursedBloodline legion) {
        removeKeyword(LEGION_OF_NIGHT);
        removeKeyword(LEGION_OF_BLOOD);

        m_bloodline = legion;
        switch (legion) {
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

        if (m_commandTrait == CommandTrait::Master_Of_The_Black_Arts) {
            if (!hasKeyword(WIZARD)) {
                addKeyword(WIZARD);
                m_totalUnbinds = 1;
                m_totalSpells = 1;
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
                m_knownSpells.push_back(std::make_unique<MysticShield>(this));
            }
        }
        if (m_commandTrait == CommandTrait::Dark_Acolyte) {
            constexpr std::array<Lore, 6> deathmageLore = {Lore::Overwhelming_Dread,Lore::Fading_Vigour,
                                                           Lore::Spectral_Grasp,Lore::Prison_Of_Grief,
                                                           Lore::Decrepify,Lore::Soul_Harvest};
            constexpr std::array<Lore, 6> vampireLore = {Lore::Blades_Of_Shyish,Lore::Spirit_Gale,
                                                         Lore::Vile_Transference,Lore::Amethystine_Pinions,
                                                         Lore::Soulpike,Lore::Amaranthine_Orb};

            if (!hasKeyword(WIZARD)) {
                addKeyword(WIZARD);
                m_totalUnbinds = 1;
                m_totalSpells = 1;
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
                m_knownSpells.push_back(std::make_unique<MysticShield>(this));
            }
            if (hasKeyword(DEATHMAGES)) {
                // TODO: make sure added spells are unique
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(deathmageLore[Dice::RollD6()], this)));
            }
            else if (hasKeyword(VAMPIRE)) {
                // TODO: make sure added spells are unique
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(vampireLore[Dice::RollD6()], this)));
            }
        }
    }

    void SoulblightBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    Wounds
    SoulblightBase::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
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

    Rerolls SoulblightBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Bane_Of_The_Living) && !target->hasKeyword(DEATH)) {
            return Rerolls::Ones;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Merciless_Hunter)) {
            return Rerolls::Ones;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int SoulblightBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        auto general = dynamic_cast<SoulblightBase *>(getRoster()->getGeneral());
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

    Rerolls SoulblightBase::chargeRerolls() const {
        auto general = dynamic_cast<SoulblightBase *>(getRoster()->getGeneral());
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

    void SoulblightBase::onStartCombat(PlayerId player) {
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

    int SoulblightBase::runModifier() const {
        auto mod = Unit::runModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Swift_Form)) {
            mod += 2;
        }
        return mod;
    }

    int SoulblightBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Curse_Of_The_Revenant)) {
            mod++;
        }
        return mod;
    }

    Rerolls SoulblightBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Deathless_Duellist) && target->hasKeyword(HERO)) {
            return Rerolls::Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int SoulblightBase::castingModifier() const {
        auto mod = Unit::castingModifier();
        if (isGeneral() && hasKeyword(WIZARD) && (m_commandTrait == CommandTrait::Master_Of_The_Black_Arts)) {
            mod++;
        }
        return mod;
    }

    int SoulblightBase::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();
        if (isGeneral() && hasKeyword(WIZARD) && (m_commandTrait == CommandTrait::Master_Of_The_Black_Arts)) {
            mod++;
        }
        return mod;
    }

    int SoulblightBase::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        auto hits = Unit::generateHits(unmodifiedHitRoll, weapon, unit);
        if ((unmodifiedHitRoll == 6) && isGeneral() && weapon->isMelee() &&
            (m_commandTrait == CommandTrait::Swift_Strikes)) {
            hits++;
        }
        return hits;
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

    int SoulblightBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        if (isGeneral() && (m_commandTrait == CommandTrait::Predator_Of_The_Shadows) && Board::Instance()->isInCover(this)) {
            mod++;
        }
        return mod;
    }

    int SoulblightBase::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        if (isGeneral() && (m_commandTrait == CommandTrait::Predator_Of_The_Shadows) && Board::Instance()->isInCover(this)) {
            mod++;
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
        SkeletonWarriors::Init();
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
        WightKingWithBlackAxe::Init();
        WightKingOnSteed::Init();
        ZombieDragon::Init();
        Zombies::Init();
    }

} //namespace Seraphon
