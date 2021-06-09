/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Seraphon.h>
#include <Roster.h>
#include "seraphon/Bastiladon.h"
#include "seraphon/ChameleonSkinks.h"
#include "seraphon/DreadSaurian.h"
#include "seraphon/EngineOfTheGods.h"
#include "seraphon/KixiTaka.h"
#include "seraphon/KlaqTrok.h"
#include "seraphon/Kroxigor.h"
#include "seraphon/LordKroak.h"
#include "seraphon/Razordons.h"
#include "seraphon/RipperdactylChief.h"
#include "seraphon/RipperdactylRiders.h"
#include "seraphon/Salamanders.h"
#include "seraphon/SaurusAstrolithBearer.h"
#include "seraphon/SaurusEternityWarden.h"
#include "seraphon/SaurusGuard.h"
#include "seraphon/SaurusKnights.h"
#include "seraphon/SaurusOldblood.h"
#include "seraphon/SaurusOldbloodCarnosaur.h"
#include "seraphon/SaurusSunblood.h"
#include "seraphon/SaurusVeteranCarnosaur.h"
#include "seraphon/SaurusVeteranColdOne.h"
#include "seraphon/SaurusWarriors.h"
#include "seraphon/SkinkPriest.h"
#include "seraphon/SkinkStarpriest.h"
#include "seraphon/SkinkStarseer.h"
#include "seraphon/Skinks.h"
#include "seraphon/SlannStarmaster.h"
#include "seraphon/StarbloodStalkers.h"
#include "seraphon/Stegadon.h"
#include "seraphon/TerradonChief.h"
#include "seraphon/TerradonRiders.h"
#include "seraphon/Troglodon.h"
#include "../AoSSimPrivate.h"
#include "Board.h"
#include "SeraphonLore.h"

namespace Seraphon {

    std::string SeraphonBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Way of the Seraphon") {
            auto wayName = magic_enum::enum_name((WayOfTheSeraphon) parameter.intValue);
            return std::string(wayName);
        } else if (std::string(parameter.name) == "Constellation") {
            auto constName = magic_enum::enum_name((Constellation) parameter.intValue);
            return std::string(constName);
        } else if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        } else if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        } else if ((std::string(parameter.name) == "Lore of Celestial Manipulation") ||
                   (std::string(parameter.name) == "Lore") ||
                   (std::string(parameter.name) == "Lore of Celestial Domination")) {
            auto loreName = magic_enum::enum_name((Lore) parameter.intValue);
            return std::string(loreName);
        }
        return ParameterValueToString(parameter);
    }

    int SeraphonBase::EnumStringToInt(const std::string &enumString) {
        auto way = magic_enum::enum_cast<WayOfTheSeraphon>(enumString);
        if (way.has_value()) return (int) way.value();

        auto constellation = magic_enum::enum_cast<Constellation>(enumString);
        if (constellation.has_value()) return (int) constellation.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        return 0;
    }

    void SeraphonBase::setWayOfTheSeraphon(Seraphon::WayOfTheSeraphon way, Constellation constellation) {
        // TODO: validate the combination of way and constellation and warn caller
        if (way == WayOfTheSeraphon::Starborne) {
            if (constellation == Constellation::Koatls_Claw || constellation == Constellation::Thunder_Lizard)
                constellation = Constellation::None;
        } else if (way == WayOfTheSeraphon::Coalesced) {
            if (constellation == Constellation::Dracothions_Tail || constellation == Constellation::Fangs_Of_Sotek)
                constellation = Constellation::None;
        }

        removeKeyword(COALESCED);
        removeKeyword(STARBORNE);
        removeKeyword(KOATLS_CLAW);
        removeKeyword(THUNDER_LIZARD);
        removeKeyword(DRACOTHIONS_TAIL);
        removeKeyword(FANGS_OF_SOTEK);

        if (way == WayOfTheSeraphon::Coalesced) addKeyword(COALESCED);
        else if (way == WayOfTheSeraphon::Starborne) addKeyword(STARBORNE);

        if (constellation == Constellation::Koatls_Claw) addKeyword(KOATLS_CLAW);
        else if (constellation == Constellation::Thunder_Lizard) addKeyword(THUNDER_LIZARD);
        else if (constellation == Constellation::Dracothions_Tail) addKeyword(DRACOTHIONS_TAIL);
        else if (constellation == Constellation::Fangs_Of_Sotek) addKeyword(FANGS_OF_SOTEK);

        m_way = way;
        m_constellation = constellation;
    }

    int SeraphonBase::moveModifier() const {
        auto mod = Unit::moveModifier();

        // First to Battle
        if ((m_battleRound == 1) && (hasKeyword(SKINK) && (hasKeyword(FANGS_OF_SOTEK)))) {
            mod += 3;
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Nimble)) {
            mod += 1;
        }
        return mod;
    }

    int SeraphonBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Savagery Incarnate
        if (m_charged && hasKeyword(KOATLS_CLAW) && hasKeyword(SAURUS)) mod++;

        return mod;
    }

    int SeraphonBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // Predatory Fighters
        if (hasKeyword(COALESCED) && (weapon->name().find("Jaws") != std::string::npos)) {
            extra++;
        }
        if ((m_commandTrait == CommandTrait::Prime_Warbeast) && (weapon->isMount())) {
            extra++;
        }
        return extra;
    }

    int SeraphonBase::braveryModifier() const {
        auto mod = Unit::braveryModifier();

        // Unfeeling
        if (hasKeyword(STARBORNE)) {
            // All Starborne units have a bravery of 10.
            mod += (10 - bravery());
        }
        return mod;
    }

    Wounds SeraphonBase::targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll,
                                                    int woundRoll) const {
        Wounds modifedWounds = Unit::targetAttackDamageModifier(wounds, attacker, hitRoll, woundRoll);

        // Scaly Skin
        if (hasKeyword(COALESCED)) {
            if (modifedWounds.normal > 1) modifedWounds.normal--;
        }
        return modifedWounds;
    }

    int SeraphonBase::woundModifier() const {
        auto mod = Unit::woundModifier();

        // Mighty Beasts of War
        if (hasKeyword(MONSTER) && hasKeyword(THUNDER_LIZARD)) mod += 2;

        return mod;
    }

    void SeraphonBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;

        if (m_commandTrait == CommandTrait::Vast_Intellect) {
            constexpr std::array<Lore, 6> loreOfDomination = {Lore::Celestial_Apotheosis,Lore::Walk_Between_Realms,
                                                              Lore::Mystical_Unforging,Lore::Celestial_Equilibrium,
                                                              Lore::Stellar_Tempest, Lore::Drain_Magic};
            // TODO: make sure added spells are unique
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(loreOfDomination[Dice::RollD6()], this)));
        }
    }

    void SeraphonBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    Rerolls SeraphonBase::castingRerolls() const {
        if ((m_commandTrait == CommandTrait::Arcane_Might) && (m_usedArcaneMightInBattleRound != m_battleRound)) {
            m_usedArcaneMightInBattleRound = m_battleRound;
            return Rerolls::Failed;
        }
        return UnitModifierInterface::castingRerolls();
    }

    void SeraphonBase::onRestore() {
        m_usedArcaneMightInBattleRound = 0;
        Unit::onRestore();
    }

    Rerolls SeraphonBase::unbindingRerolls() const {
        if ((m_commandTrait == CommandTrait::Arcane_Might) && (m_usedArcaneMightInBattleRound != m_battleRound)) {
            m_usedArcaneMightInBattleRound = m_battleRound;
            return Rerolls::Failed;
        }
        return Unit::unbindingRerolls();
    }

    void SeraphonBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (isGeneral() && (remainingModels() > 0) && ((m_commandTrait == CommandTrait::Great_Rememberer) ||
                                                       (m_commandTrait == CommandTrait::Mighty_Warleader))) {
            if (Dice::RollD6() >= 4) {
                getRoster()->addCommandPoints(1);
            }
        }
        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Old_And_Grizzled)) {
            if (Dice::RollD6() >= 3) {
                getRoster()->addCommandPoints(1);
            }
        }
    }

    Rerolls SeraphonBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Disciplined_Fury) && weapon->isMelee()) {
            return Rerolls::Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls SeraphonBase::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Thickly_Scaled_Hide)) {
            return Rerolls::Ones;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int SeraphonBase::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);
        if (isGeneral() && (m_commandTrait == CommandTrait::Nimble)) {
            mod++;
        }
        return mod;
    }

    int SeraphonBase::castingModifier() const {
        auto mod = Unit::castingModifier();

        if (hasKeyword(WIZARD) && (m_commandTrait == CommandTrait::Master_Of_Star_Rituals)) {
            mod++;
        }
        return mod;
    }

    void SeraphonBase::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (m_commandTrait == CommandTrait::Cunning) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
            if (hero && (Dice::RollD6() >= 4)) {
                hero->applyDamage({0, 1, Wounds::Source::Ability}, this);
            }
        }
    }

    void Init() {
        Bastiladon::Init();
        ChameleonSkinks::Init();
        DreadSaurian::Init();
        EngineOfTheGods::Init();
        KixiTakaTheDiviner::Init();
        KlaqTrok::Init();
        Kroxigor::Init();
        LordKroak::Init();
        Razordons::Init();
        RipperdactylChief::Init();
        RipperdactylRiders::Init();
        Salamanders::Init();
        SaurusAstrolithBearer::Init();
        SaurusEternityWarden::Init();
        SaurusGuard::Init();
        SaurusKnights::Init();
        SaurusOldblood::Init();
        SaurusOldbloodOnCarnosaur::Init();
        SaurusScarVeteranOnCarnosaur::Init();
        SaurusScarVeteranOnColdOne::Init();
        SaurusSunblood::Init();
        SaurusWarriors::Init();
        SkinkPriest::Init();
        SkinkStarpriest::Init();
        SkinkStarseer::Init();
        Skinks::Init();
        SlannStarmaster::Init();
        StarbloodStalkers::Init();
        Stegadon::Init();
        TerradonChief::Init();
        TerradonRiders::Init();
        Troglodon::Init();
    }

    class GiftFromTheHeavens : public CommandAbility {
    public:
        explicit GiftFromTheHeavens(Unit *general) :
                CommandAbility(general, "Gift from the Heavens", 18, 18, GamePhase::Hero) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_targetKeywords = {SERAPHON};
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        bool apply(Unit *target) override {
            if (target == nullptr) return false;

            target->buffMovement(MovementRule::Can_Fly, true, defaultDuration());
            target->buffModifier(Attribute::To_Save_Missile, 1, defaultDuration());
            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    CommandAbility *CreateGiftFromTheHeavens(Unit *general) {
        return new GiftFromTheHeavens(general);
    }


    class CometsCall : public Spell {
    public:
        explicit CometsCall(Unit *caster) :
                Spell(caster, "Comet's Call", 7, 1000) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::AreaOfEffectDamage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            return apply(castingRoll, unmodifiedCastingRoll, 0.0, 0.0);
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override {

            auto units = Board::Instance()->getAllUnits(GetEnemyId(m_caster->owningPlayer()));
            auto numTargets = (castingRoll >= 10) ? Dice::RollD6() : Dice::RollD3();
            numTargets = std::min(numTargets, (int) units.size());

            Wounds wounds{0, Dice::RollD3(), Wounds::Source::Spell};
            for (int i = 0; i < numTargets; i++) {
                units[i]->applyDamage(wounds, m_caster);
            }
            return Spell::Result::Success;
        }
    };

    Spell *CreateCometsCall(Unit *caster) {
        return new CometsCall(caster);
    }

} //namespace Seraphon
