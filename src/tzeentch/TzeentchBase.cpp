/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzeentchBase.h>
#include <Board.h>
#include <Roster.h>
#include <magic_enum.hpp>
#include "tzeentch/KairicAcolytes.h"
#include "tzeentch/HorrorsOfTzeentch.h"
#include "tzeentch/TzaangorEnlightened.h"
#include "tzeentch/TzaangorEnlightenedOnDisks.h"
#include "tzeentch/Tzaangors.h"
#include "tzeentch/FlamersOfTzeentch.h"
#include "tzeentch/ExaltedFlamersOfTzeentch.h"
#include "tzeentch/TzaangorSkyfires.h"
#include "tzeentch/TzaangorShaman.h"
#include "tzeentch/ScreamersOfTzeentch.h"
#include "tzeentch/LordOfChange.h"
#include "tzeentch/KairosFateweaver.h"
#include "tzeentch/MagisterOnDisc.h"
#include "tzeentch/Magister.h"
#include "tzeentch/GauntSummoner.h"
#include "tzeentch/OgroidThaumaturge.h"
#include "tzeentch/TheChangeling.h"
#include "tzeentch/Curseling.h"
#include "tzeentch/Fateskimmer.h"
#include "tzeentch/Vortemis.h"
#include "tzeentch/Fatemaster.h"
#include "tzeentch/BlueScribes.h"
#include "tzeentch/Changecaster.h"
#include "tzeentch/Fluxmaster.h"
#include "tzeentch/TheEyesOfNine.h"
#include "TzeentchSpells.h"

namespace Tzeentch {

    DestinyDice TzeentchBase::s_destinyDice;

    std::string TzeentchBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Change Coven") {
            auto coven = magic_enum::enum_name((ChangeCoven) parameter.intValue);
            return std::string(coven);
        }
        if (std::string(parameter.name) == "Lore") {
            auto lore = magic_enum::enum_name((Lore) parameter.intValue);
            return std::string(lore);
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

    int TzeentchBase::EnumStringToInt(const std::string &enumString) {

        auto coven = magic_enum::enum_cast<ChangeCoven>(enumString);
        if (coven.has_value()) return (int) coven.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        return 0;
    }

    void TzeentchBase::setChangeCoven(ChangeCoven coven) {
        removeKeyword(ETERNAL_CONFLAGRATION);
        removeKeyword(HOSTS_DUPLICITOUS);
        removeKeyword(HOSTS_ARCANUM);
        removeKeyword(CULT_OF_THE_TRANSIENT_FORM);
        removeKeyword(PYROFANE_CULT);
        removeKeyword(GUILD_OF_SUMMONERS);
        removeKeyword(UNBOUND_FLUX);
        removeKeyword(CULT_OF_A_THOUSAND_EYES);

        m_coven = coven;
        switch (coven) {
            case ChangeCoven::Eternal_Conflagration:
                addKeyword(ETERNAL_CONFLAGRATION);
                break;
            case ChangeCoven::Hosts_Duplicitous:
                addKeyword(HOSTS_DUPLICITOUS);
                break;
            case ChangeCoven::Hosts_Arcanum:
                addKeyword(HOSTS_ARCANUM);
                break;
            case ChangeCoven::Cult_Of_The_Transient_Form:
                addKeyword(CULT_OF_THE_TRANSIENT_FORM);
                break;
            case ChangeCoven::Pyrofane_Cult:
                addKeyword(PYROFANE_CULT);
                break;
            case ChangeCoven::Guild_Of_Summoners:
                addKeyword(GUILD_OF_SUMMONERS);
                break;
            case ChangeCoven::Unbound_Flux:
                addKeyword(UNBOUND_FLUX);
                break;
            case ChangeCoven::Cult_Of_A_Thousand_Eyes:
                addKeyword(CULT_OF_A_THOUSAND_EYES);
                break;
            default:
                break;
        }
    }

    int TzeentchBase::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Twisters of Materiality
        if (hasKeyword(ETERNAL_CONFLAGRATION) &&
            (weapon->name().find("Warpflame") || weapon->name().find("Magical Flames"))) {
            return weapon->rend() - 1;
        }
        return Unit::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int TzeentchBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Arrows of Tzeentch
        if (hasKeyword(PYROFANE_CULT) && hasKeyword(KAIRIC_ACOLYTES) && (weapon->name() == "Sorcerous Bolt")) {
            mod++;
        }
        return mod;
    }

    int TzeentchBase::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);

        // Locus of Change
        if (!weapon->isMissile() && hasKeyword(DAEMON)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if (unit->hasKeyword(HERO) && unit->hasKeyword(DAEMON)) {
                    mod--;
                    break;
                }
            }
        }

        // Coruscating Flames
        if (weapon->isMissile() && hasKeyword(ETERNAL_CONFLAGRATION) && hasKeyword(DAEMON)) {
            auto general = getRoster()->getGeneral();
            if (general && (general->remainingModels() > 0) && (distanceTo(general) < 12.0)) {
                auto tzeench = dynamic_cast<TzeentchBase *>(general);
                if (tzeench && (tzeench->m_commandTrait == CommandTrait::Coruscating_Flames))
                    mod--;
            }
        }

        if (isGeneral() && weapon->isMissile() && (m_commandTrait == CommandTrait::Shrouded_In_Unnatural_Flame)) {
            mod--;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Illusionist)) {
            mod--;
        }

        return mod;
    }

    void TzeentchBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;

        if (trait == CommandTrait::Arch_Sorcerer) {
            if (hasKeyword(ARCANITE)) {
                constexpr std::array<Lore, 6> loreOfFate = {Lore::Bolt_Of_Tzeentch,Lore::Arcane_Suggestion,
                                                            Lore::Glimpse_The_Future,Lore::Shield_Of_Faith,
                                                            Lore::Infusion_Arcanum,Lore::Treacherous_Bond};
                // TODO: make sure added spells are unique
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(loreOfFate[Dice::RollD6()], this)));
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(loreOfFate[Dice::RollD6()], this)));
            }
            else if (hasKeyword(DAEMON)) {
                constexpr std::array<Lore, 6> loreOfChange = {Lore::Bolt_Of_Tzeentch,Lore::Treason_Of_Tzeentch,
                                                              Lore::Arcane_Transformation,Lore::Unchecked_Mutation,
                                                              Lore::Fold_Reality,Lore::Tzeentchs_Firestorm};
                // TODO: make sure added spells are unique
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(loreOfChange[Dice::RollD6()], this)));
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(loreOfChange[Dice::RollD6()], this)));
            }
        }
    }

    void TzeentchBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void TzeentchBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            if (isGeneral() && (remainingModels() > 0)) {
                if (m_commandTrait == CommandTrait::Boundless_Mutation) {
                    if (Dice::RollD6() >= 2)
                        heal(Dice::RollD3());
                }

                if (!m_usedDaemonspark && (m_commandTrait == CommandTrait::Daemonspark)) {
                    getRoster()->incrementResource(3);
                    m_usedDaemonspark = true;
                }

                if (m_commandTrait == CommandTrait::Nexus_Of_Fate) {
                    int roll = Dice::RollD6();
                    if ((roll >= 5) || (roll == 1)) {
                        // Discard 2's, 3's and 4's.
                        if (s_destinyDice.have(2)) {
                            s_destinyDice.replace(2, roll);
                        } else if (s_destinyDice.have(3)) {
                            s_destinyDice.replace(3, roll);
                        } else if (s_destinyDice.have(4)) {
                            s_destinyDice.replace(4, roll);
                        }
                    }
                }

                if (m_commandTrait == CommandTrait::Prophet_Of_The_Ostensible) {
                    if (Dice::RollD6() >= 4)
                        getRoster()->addCommandPoints(1);
                }
            }
        }
    }

    int TzeentchBase::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);

        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Aether_Tether)) {
            mod++;
        }
        return mod;
    }

    Wounds TzeentchBase::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;

        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Incorporeal_Form)) {
            if (wounds.source == Wounds::Source::Spell) {
                totalWounds.normal = 0;
                totalWounds.mortal = 0;
            }
        }

        return Unit::applyWoundSave(totalWounds, attackingUnit);
    }

    double TzeentchBase::unbindingDistance() const {
        if (isGeneral() && (remainingModels() > 0) && (m_commandTrait == CommandTrait::Magical_Supremancy)) {
            return Unit::unbindingDistance() + 12.0;
        }
        return Unit::unbindingDistance();
    }

    Rerolls TzeentchBase::castingRerolls() const {
        // Will of the Phantom Lord
        if (hasKeyword(HOSTS_DUPLICITOUS) && hasKeyword(DAEMON) && hasKeyword(WIZARD)) {
            auto general = getRoster()->getGeneral();
            if (general && (general->remainingModels() > 0) && (distanceTo(general) < 9.0)) {
                auto tzeentch = dynamic_cast<TzeentchBase *>(general);
                if (tzeentch && (tzeentch->m_commandTrait == CommandTrait::Will_Of_The_Phantom_Lord)) {
                    return Rerolls::Failed;
                }
            }
        }
        return Unit::castingRerolls();
    }

    Rerolls TzeentchBase::unbindingRerolls() const {
        // Will of the Phantom Lord
        if (hasKeyword(HOSTS_DUPLICITOUS) && hasKeyword(DAEMON) && hasKeyword(WIZARD)) {
            auto general = getRoster()->getGeneral();
            if (general && (general->remainingModels() > 0) && (distanceTo(general) < 9.0)) {
                auto tzeentch = dynamic_cast<TzeentchBase *>(general);
                if (tzeentch && (tzeentch->m_commandTrait == CommandTrait::Will_Of_The_Phantom_Lord)) {
                    return Rerolls::Failed;
                }
            }
        }
        return Unit::unbindingRerolls();
    }

    int TzeentchBase::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        // Defiant in their Pursuit
        if (hasKeyword(CULT_OF_THE_TRANSIENT_FORM)) {
            auto general = getRoster()->getGeneral();
            if (general && (general->remainingModels() > 0) && (distanceTo(general) < 12.0)) {
                auto tzeentch = dynamic_cast<TzeentchBase *>(general);
                if (tzeentch && (tzeentch->m_commandTrait == CommandTrait::Defiant_In_Their_Pursuit)) {
                    mod += 2;
                }
            }
        }
        return mod;
    }

    bool TzeentchBase::battleshockRequired() const {
        // Aegis of Insanity
        if (hasKeyword(UNBOUND_FLUX) && (hasKeyword(DAEMON))) {
            auto general = getRoster()->getGeneral();
            if (general && (general->remainingModels() > 0) && (distanceTo(general) < 9.0)) {
                auto tzeentch = dynamic_cast<TzeentchBase *>(general);
                if (tzeentch && (tzeentch->m_commandTrait == CommandTrait::Aegis_Of_Insanity)) {
                    return false;
                }
            }
        }
        return Unit::battleshockRequired();
    }

    Wounds TzeentchBase::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        if (isGeneral() && (m_commandTrait == CommandTrait::Soul_Burn) && weapon->isMelee() && (hitRoll == 6)) {
            damage.mortal++;
        }
        return damage;
    }

    void Init() {
        CurselingEyeOfTzeentch::Init();
        ExaltedFlamersOfTzeentch::Init();
        Fatemaster::Init();
        Fateskimmer::Init();
        FlamersOfTzeentch::Init();
        GauntSummonerOfTzeentch::Init();
        HorrorsOfTzeentch::Init();
        KairicAcolytes::Init();
        KairosFateweaver::Init();
        LordOfChange::Init();
        Magister::Init();
        MagisterOnDiscOfTzeentch::Init();
        OgroidThaumaturge::Init();
        ScreamersOfTzeentch::Init();
        TheChangeling::Init();
        TzaangorEnlightened::Init();
        TzaangorEnlightenedOnDisks::Init();
        TzaangorShaman::Init();
        TzaangorSkyfires::Init();
        Tzaangors::Init();
        VortemisTheAllSeeing::Init();
        TheBlueScribes::Init();
        ChangecasterHeraldOfTzeentch::Init();
        FluxmasterHeraldOfTzeentchOnDisc::Init();
        TheEyesOfTheNine::Init();
    }

} //namespace Tzeentch
