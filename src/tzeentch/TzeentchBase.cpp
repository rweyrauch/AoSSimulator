/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzeentchBase.h>
#include <Board.h>
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

namespace Tzeentch {

    std::string TzeentchBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Change Coven") {
            auto coven = magic_enum::enum_name((ChangeCoven)parameter.intValue);
            return std::string(coven);
        }
        if (std::string(parameter.name) == "Lore") {
            auto lore = magic_enum::enum_name((Lore)parameter.intValue);
            return std::string(lore);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait)parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact)parameter.intValue);
            return std::string(artefactName);
        }

        return ParameterValueToString(parameter);
    }

    int TzeentchBase::EnumStringToInt(const std::string &enumString) {

        auto coven = magic_enum::enum_cast<ChangeCoven>(enumString);
        if (coven.has_value()) return (int)coven.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

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

    int TzeentchBase::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Twisters of Materiality
        if (hasKeyword(ETERNAL_CONFLAGRATION) &&
            (weapon->name().find("Warpflame") || weapon->name().find("Magical Flames"))) {
            return weapon->rend() - 1;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
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
        return mod;
    }

    void TzeentchBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void TzeentchBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
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
