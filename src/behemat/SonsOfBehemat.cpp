/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <behemat/SonsOfBehemat.h>
#include <behemat/Mancrusher.h>
#include <behemat/Gatebreaker.h>
#include <behemat/KrakenEater.h>
#include <behemat/Warstomper.h>
#include <magic_enum.hpp>

namespace SonsOfBehemat {

    std::string SonsOfBehematBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Tribe") {
            auto tribeName = magic_enum::enum_name((Tribe) parameter.intValue);
            return std::string(tribeName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        }
        if (std::string(parameter.name) == "Fierce Loathing") {
            auto loathingName = magic_enum::enum_name((FierceLoathing) parameter.intValue);
            return std::string(loathingName);
        }
        return ParameterValueToString(parameter);
    }

    int SonsOfBehematBase::EnumStringToInt(const std::string &enumString) {

        auto tribe = magic_enum::enum_cast<Tribe>(enumString);
        if (tribe.has_value()) return (int) tribe.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto loathing = magic_enum::enum_cast<FierceLoathing>(enumString);
        if (loathing.has_value()) return (int) loathing.value();

        return 0;
    }

    void SonsOfBehematBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void SonsOfBehematBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    int SonsOfBehematBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        switch (m_fierceLoathing) {
            case FierceLoathing::Bossy_Pants_And_Clever_Clogs:
                if (target->hasKeyword(HERO) || target->hasKeyword(WIZARD)) mod++;
                break;
            case FierceLoathing::Idiots_With_Flags:
                if (target->hasKeyword(TOTEM)) mod++;
                break;
            case FierceLoathing::Shiny_Uns:
                if (!target->hasKeyword(HERO) && !target->hasKeyword(MONSTER)) {
                    if (target->save() <= 4) mod++;
                }
                break;
            case FierceLoathing::Crowds:
                if (target->remainingModels() >= 20) mod++;
                break;
            case FierceLoathing::Wannabes:
                if (target->hasKeyword(MONSTER) || target->hasKeyword(WAR_MACHINE)) mod++;
                break;
            case FierceLoathing::Piggybackers:
                if (target->hasMount() && !target->hasKeyword(MONSTER)) mod++;
                break;
            default:
                break;
        }

        return mod;
    }

    int SonsOfBehematBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Monstrously_Tough)) mod += 5;
        return mod;
    }

    Rerolls SonsOfBehematBase::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Old_And_Gnarly)) return Rerolls::Ones;
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int SonsOfBehematBase::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        if (isGeneral() && (m_commandTrait == CommandTrait::Extremely_Intimidating)) {
            if (distanceTo(attacker) < 3.0) mod--;
        }
        return mod;
    }

    void Init() {
        Mancrusher::Init();
        Gatebreaker::Init();
        KrakenEater::Init();
        Warstomper::Init();
    }

} //namespace SonsOfBehemat

