/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/StormcastEternals.h>
#include <Board.h>
#include <Roster.h>
#include "stormcast/Liberators.h"
#include "stormcast/CelestarBallista.h"
#include "stormcast/Evocators.h"
#include "stormcast/Judicators.h"
#include "stormcast/Sequitors.h"
#include "stormcast/LordOrdinator.h"
#include "stormcast/Retributors.h"
#include "stormcast/LordCelestant.h"
#include "stormcast/LordCelestantOnDracoth.h"
#include "stormcast/LordCelestantOnStardrake.h"
#include "stormcast/KnightIncantor.h"
#include "stormcast/KnightQuestor.h"
#include "stormcast/KnightZephyros.h"
#include "stormcast/VanguardHunters.h"
#include "stormcast/VanguardRaptorsHurricane.h"
#include "stormcast/VanguardRaptorsLongstrike.h"
#include "stormcast/Castigators.h"
#include "stormcast/VandusHammerhand.h"
#include "stormcast/GryphHounds.h"
#include "stormcast/Concussors.h"
#include "stormcast/Desolators.h"
#include "stormcast/Fulminators.h"
#include "stormcast/Tempestors.h"
#include "stormcast/EvocatorsOnDracolines.h"
#include "stormcast/Prosecutors.h"
#include "stormcast/Protectors.h"
#include "stormcast/Decimators.h"
#include "stormcast/NeaveBlacktalon.h"
#include "stormcast/GavrielSureheart.h"
#include "stormcast/LordExorcist.h"
#include "stormcast/LordRelictor.h"
#include "stormcast/LordArcanumOnDracoline.h"
#include "stormcast/LordArcanumOnGryphcharger.h"
#include "stormcast/LordArcanumOnTauralon.h"
#include "stormcast/LordArcanum.h"
#include "stormcast/LordAquilor.h"
#include "stormcast/LordCastellant.h"
#include "stormcast/LordVeritant.h"
#include "stormcast/KnightAzyros.h"
#include "stormcast/KnightHeraldor.h"
#include "stormcast/KnightVenator.h"
#include "stormcast/KnightVexillor.h"
#include "stormcast/AventisFirestrike.h"
#include "stormcast/AstreiaSolbright.h"
#include "stormcast/VanguardPalladors.h"
#include "stormcast/DrakeswornTemplar.h"
#include "stormcast/CelestantPrime.h"
#include "stormcast/Aetherwings.h"
#include "stormcast/SteelheartsChampions.h"
#include "stormcast/TheFarstriders.h"
#include "stormcast/StormsiresCursebreakers.h"
#include "stormcast/AveronStormsire.h"
#include "stormcast/LynusGhalmorian.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {

    StormcastEternal::StormcastEternal(Stormhost host, const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
            Unit(name, move, wounds, bravery, save, fly) {
        setStormhost(host);
        s_globalBraveryMod.connect(this, &StormcastEternal::deathlyAura, &m_deathlyAuraConnection);
    }

    StormcastEternal::~StormcastEternal() {
        m_deathlyAuraConnection.disconnect();
    }

    int StormcastEternal::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = Unit::toHitModifier(weapon, unit);

        // Beast Stalkers
        if (hasKeyword(ASTRAL_TEMPLARS) && unit->hasKeyword(MONSTER)) {
            modifier += 1;
        }

        return modifier;
    }

    Rerolls StormcastEternal::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Driven by Vengeance
        if (hasKeyword(CELESTIAL_VINDICATORS) && m_charged) {
            return Rerolls::Ones;
        }

        // Portents and Omens
        if (isGeneral() && (m_commandTrait == CommandTrait::Portents_And_Omens) && !m_usedPortentsAndOmensInTurn) {
            m_usedPortentsAndOmensInTurn = true;
            return Rerolls::Failed;
        }
        return Unit::toHitRerolls(weapon, unit);
    }

    int StormcastEternal::braveryModifier() const {
        int modifier = Unit::braveryModifier();

        // First to be Forged
        if (hasKeyword(HAMMERS_OF_SIGMAR)) {
            modifier += 1;
        }

        // Shield of Civilisation
        auto liberators = Board::Instance()->getUnitsWithKeyword(owningPlayer(), LIBERATORS);
        for (auto unit : liberators) {
            if (distanceTo(unit) < 12.0) {
                modifier += 1;
                break;
            }
        }
        return modifier;
    }

    Rerolls StormcastEternal::battleshockRerolls() const {
        // No True Death
        if (hasKeyword(ANVILS_OF_THE_HELDENHAMMER)) {
            return Rerolls::Failed;
        }

        return Unit::battleshockRerolls();
    }

    void StormcastEternal::setStormhost(Stormhost host) {
        removeKeyword(HAMMERS_OF_SIGMAR);
        removeKeyword(HALLOWED_KNIGHTS);
        removeKeyword(CELESTIAL_VINDICATORS);
        removeKeyword(ANVILS_OF_THE_HELDENHAMMER);
        removeKeyword(KNIGHTS_EXCELSIOR);
        removeKeyword(CELESTIAL_WARBRINGERS);
        removeKeyword(TEMPEST_LORDS);
        removeKeyword(ASTRAL_TEMPLARS);

        m_stormHost = host;
        switch (m_stormHost) {
            case Stormhost::Hammers_Of_Sigmar:
                addKeyword(HAMMERS_OF_SIGMAR);
                break;
            case Stormhost::Hallowed_Knights:
                addKeyword(HALLOWED_KNIGHTS);
                break;
            case Stormhost::Celestial_Vindicators:
                addKeyword(CELESTIAL_VINDICATORS);
                break;
            case Stormhost::Anvils_Of_The_Heldenhammer:
                addKeyword(ANVILS_OF_THE_HELDENHAMMER);
                break;
            case Stormhost::Knights_Excelsior:
                addKeyword(KNIGHTS_EXCELSIOR);
                break;
            case Stormhost::Celestial_Warbringers:
                addKeyword(CELESTIAL_WARBRINGERS);
                break;
            case Stormhost::Tempest_Lords:
                addKeyword(TEMPEST_LORDS);
                break;
            case Stormhost::Astral_Templars:
                addKeyword(ASTRAL_TEMPLARS);
                break;
            default:
                break;
        }
    }

    std::string StormcastEternal::ValueToString(const Parameter &parameter) {
        if ((std::string(parameter.name) == "Lore of the Storm") ||
            (std::string(parameter.name) == "Lore of Invigoration") ||
            (std::string(parameter.name) == "Lore")) {
            return ToString((Lore) parameter.intValue);
        }
        if (std::string(parameter.name) == "Stormhost") {
            auto hostName = magic_enum::enum_name((Stormhost) parameter.intValue);
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
        if (std::string(parameter.name) == "Mount Trait") {
            auto traitName = magic_enum::enum_name((MountTrait) parameter.intValue);
            return std::string(traitName);
        }
        return ParameterValueToString(parameter);
    }

    int StormcastEternal::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) return ToInteger(lore);

        auto stormhost = magic_enum::enum_cast<Stormhost>(enumString);
        if (stormhost.has_value()) return (int) stormhost.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto mount = magic_enum::enum_cast<MountTrait>(enumString);
        if (mount.has_value()) return (int) mount.value();

        return 0;
    }

    void StormcastEternal::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Grand Strategists
        if ((owningPlayer() == player) && hasKeyword(TEMPEST_LORDS)) {
            if (Dice::RollD6() >= 4) {
                m_roster->addCommandPoints(1);
            }
        }

        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Consummate_Commander)) {
            if (Dice::RollD6() >= 4) {
                m_roster->addCommandPoints(1);
            }
        }

        if ((owningPlayer() == player) && hasKeyword(ANVILS_OF_THE_HELDENHAMMER)) {
            if (getCommandPoints() > 0) {
                auto range = isGeneral() ? 18.0 : 9.0;
                auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), range);
                for (auto unit : units) {
                    if (unit->remainingModels() > 0) {
                        const Weapon *preferredWeapon = unit->getWeapon(0);
                        if (preferredWeapon != nullptr) {
                            for (auto i = 0; i < unit->getNumWeapons(); i++) {
                                if (unit->getWeapon(i)->strength() > preferredWeapon->strength()) {
                                    preferredWeapon = unit->getWeapon(i);
                                }
                            }
                            int numSlain = 0;
                            if (preferredWeapon->isMissile() && unit->shootingTarget()) {
                                if (unit->distanceTo(unit->shootingTarget()) < preferredWeapon->range()) {
                                    unit->shoot(-1, unit->shootingTarget(), numSlain);
                                    break;
                                }
                            } else if (preferredWeapon->isMelee() && unit->meleeTarget()) {
                                if (unit->distanceTo(unit->meleeTarget()) < preferredWeapon->range()) {
                                    unit->fight(-1, unit->meleeTarget(), numSlain);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void StormcastEternal::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void StormcastEternal::setCommandTrait(CommandTrait commandTrait) {
        m_commandTrait = commandTrait;
    }

    Wounds StormcastEternal::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;
        if (isGeneral() && (m_commandTrait == CommandTrait::Shielded_By_Faith)) {
            Dice::RollResult rolls;
            Dice::RollD6(wounds.mortal, rolls);
            totalWounds.mortal -= rolls.rollsGE(5);
        }

        // We Cannot Fail
        if (m_stormHost == Stormhost::Hammers_Of_Sigmar) {
            auto general = getRoster()->getGeneral();
            if (general && general->hasKeyword(HAMMERS_OF_SIGMAR)) {
                auto sceGeneral = dynamic_cast<StormcastEternal *>(general);
                if (sceGeneral && (sceGeneral->remainingModels() > 0) &&
                    (sceGeneral->m_commandTrait == CommandTrait::We_Cannot_Fail) && distanceTo(general) < 9.0) {
                    totalWounds = ignoreWounds(totalWounds, 6);
                }
            }
        }

        // Only the Faithful
        if ((m_stormHost == Stormhost::Hallowed_Knights) && (wounds.source == Wounds::Source::Spell)) {
            Dice::RollResult rolls;
            Dice::RollD6(wounds.mortal, rolls);
            totalWounds.mortal -= rolls.rollsGE(6);
        }

        return Unit::applyWoundSave(totalWounds, attackingUnit);
    }

    Wounds StormcastEternal::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        Wounds damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Single-minded Fury
        if (isGeneral() && (m_stormHost == Stormhost::Celestial_Vindicators) && (hitRoll == 6)) {
            damage.normal++;
        }

        // Divine Executioner
        if (isGeneral() && (m_stormHost == Stormhost::Knights_Excelsior) && target->hasKeyword(HERO)) {
            damage.normal++;
        }
        return damage;
    }

    Rerolls StormcastEternal::chargeRerolls() const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Zealous_Crusader)) {
            return Rerolls::Failed;
        }
        return Unit::chargeRerolls();
    }

    int StormcastEternal::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);

        // Staunch Defender
        if (!charged()) {
            auto general = dynamic_cast<StormcastEternal *>(getRoster()->getGeneral());
            if (general && (general->remainingModels() > 0) &&
                (general->m_commandTrait == CommandTrait::Staunch_Defender) && (distanceTo(general) < 9.0)) {
                mod++;
            }
        }
        return mod;
    }

    int StormcastEternal::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        if (isGeneral() && (m_commandTrait == CommandTrait::Champion_Of_The_Realms) &&
            weapon->isFlagSet(Weapon::Preferred)) {
            attacks++;
        }
        return attacks;
    }

    int StormcastEternal::deathlyAura(const Unit *unit) {
        if (isGeneral() && (m_commandTrait == CommandTrait::Deathly_Aura) && (distanceTo(unit) < 6)) {
            return -1;
        }
        return 0;
    }

    int StormcastEternal::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        if (isGeneral() && (m_commandTrait == CommandTrait::Bonds_Of_Noble_Duty) && weapon->isMelee()) {
            auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), TEMPEST_LORDS, 6.0);
            if (unit && (distanceTo(unit) < 6.0)) {
                mod++;
            }
        }
        return mod;
    }

    void StormcastEternal::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        Unit::onFriendlyModelSlain(numSlain, attacker, source);

        if (isGeneral() && (m_commandTrait == CommandTrait::Martyrs_Strength)) {
            if (Dice::RollD6() >= 2) {
                int numEnemiesSlain = 0;
                doPileIn();
                fight(numSlain, attacker, numEnemiesSlain);
            }
        }
    }

    void StormcastEternal::onFriendlyUnitSlain(const Unit *attacker) {
        Unit::onFriendlyUnitSlain(attacker);

        // Soul of the Stormhost
        if (hasKeyword(HAMMERS_OF_SIGMAR) && hasKeyword(REDEEMER) && (getRoster()->getCommandPoints() > 0)) {
            auto general = dynamic_cast<StormcastEternal *>(getRoster()->getGeneral());
            if (general && (general->remainingModels() > 0) && (general->hasKeyword(HAMMERS_OF_SIGMAR))) {
                if (Dice::RollD6() >= 5) {
                    returnModels(numModels());
                    // TODO: redeploy - anywhere 9" from enemy models
                    deploy(position(), orientation());
                }
                getRoster()->useCommandPoint();
            }
        }
    }

    void StormcastEternal::onEnemyUnitSlain(const Unit *enemyUnit) {
        Unit::onEnemyUnitSlain(enemyUnit);

        // Storm of Annihilation
        if (hasKeyword(KNIGHTS_EXCELSIOR)) {
            buffReroll(Attribute::To_Hit_Melee, Rerolls::Ones, {Phase::Hero, DurationRestOfGame, owningPlayer()});
            buffReroll(Attribute::To_Hit_Missile, Rerolls::Ones, {Phase::Hero, DurationRestOfGame, owningPlayer()});
        }
    }

    void StormcastEternal::onBeginTurn(int battleRound, PlayerId player) {
        Unit::onBeginTurn(battleRound, player);

        m_usedPortentsAndOmensInTurn = false;
    }

    Rerolls StormcastEternal::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Portents and Omens
        if (isGeneral() && (m_commandTrait == CommandTrait::Portents_And_Omens) && !m_usedPortentsAndOmensInTurn) {
            m_usedPortentsAndOmensInTurn = true;
            return Rerolls::Failed;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    Rerolls StormcastEternal::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Portents and Omens
        if (isGeneral() && (m_commandTrait == CommandTrait::Portents_And_Omens) && !m_usedPortentsAndOmensInTurn) {
            m_usedPortentsAndOmensInTurn = true;
            return Rerolls::Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    void Init() {
        CelestantPrime::Init();
        AventisFirestrike::Init();
        AstreiaSolbright::Init();
        VandusHammerhand::Init();
        NeaveBlacktalon::Init();
        GavrielSureheart::Init();
        SteelheartsChampions::Init();
        TheFarstriders::Init();
        AveronStormsire::Init();
        StormsiresCursebreakers::Init();
        LordArcanumOnDracoline::Init();
        LordArcanumOnGryphcharger::Init();
        LordArcanumOnTauralon::Init();
        LordArcanum::Init();
        LordAquilor::Init();
        LordCelestant::Init();
        LordCelestantOnDracoth::Init();
        LordCelestantOnStardrake::Init();
        LordCastellant::Init();
        LordOrdinator::Init();
        LordExorcist::Init();
        LordRelictor::Init();
        LordVeritant::Init();
        KnightAzyros::Init();
        KnightHeraldor::Init();
        KnightQuestor::Init();
        KnightIncantor::Init();
        KnightVenator::Init();
        KnightVexillor::Init();
        KnightZephyros::Init();
        GryphHounds::Init();
        DrakeswornTemplar::Init();
        Concussors::Init();
        Desolators::Init();
        Fulminators::Init();
        Tempestors::Init();
        Prosecutors::Init();
        Decimators::Init();
        Protectors::Init();
        Retributors::Init();
        Liberators::Init();
        Judicators::Init();
        VanguardRaptorsHurricane::Init();
        VanguardRaptorsLongstrike::Init();
        VanguardHunters::Init();
        VanguardPalladors::Init();
        Aetherwings::Init();
        Castigators::Init();
        CelestarBallista::Init();
        Sequitors::Init();
        Evocators::Init();
        EvocatorsOnCelestialDracolines::Init();
        LynusGhalmorianOnGryphcharger::Init();
    }

    bool DoSpiritFlasks(Unit *owner) {
        auto units = Board::Instance()->getUnitsWithin(owner, GetEnemyId(owner->owningPlayer()), 3.0);
        int potentialWoundsPerFlask = 0;
        for (auto ip : units) {
            if (ip->remainingModels() >= 10)
                potentialWoundsPerFlask += 3;
            else
                potentialWoundsPerFlask++;
        }

        int numFlasks = 0;
        if (potentialWoundsPerFlask > owner->remainingWounds() * 6) {
            numFlasks = 3;
        } else if (potentialWoundsPerFlask > owner->remainingWounds() * 4) {
            numFlasks = 2;
        } else if (potentialWoundsPerFlask > owner->remainingWounds() * 2) {
            numFlasks = 1;
        }

        // Going out with a bang!
        if (owner->remainingWounds() == 1) {
            numFlasks = 3;
        }

        if (numFlasks) {
            for (auto ip : units) {
                int damage = 1;
                if (ip->remainingModels() >= 10) damage = RAND_D3;

                int mortalsTarget = 0;
                for (auto i = 0; i < numFlasks; i++) {
                    mortalsTarget += Dice::RollSpecial(damage);
                }

                auto numSlain = ip->applyDamage({0, mortalsTarget}, owner);

                PLOG_INFO.printf("%s shattered %d Spirit Flasks inflicting %d mortal wounds on %s slaying %d.\n",
                                 owner->name().c_str(), numFlasks, mortalsTarget, ip->name().c_str(), numSlain);
            }

            int mortalsSelf = numFlasks;
            auto dead = owner->applyDamage({0, mortalsSelf}, owner);
            PLOG_INFO << "Spirit Flasks inflicted " << mortalsSelf << " wounds on " << owner->name() << ".  Slaying "
                      << dead << " models.";
        }

        return (numFlasks != 0);
    }

} // namespace StormcastEternals
