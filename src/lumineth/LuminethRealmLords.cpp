/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/LuminethRealmLords.h>
#include <magic_enum.hpp>
#include "lumineth/AuralanWardens.h"
#include "lumineth/AuralanSentinels.h"
#include "lumineth/Dawnriders.h"
#include "lumineth/Eltharion.h"
#include "lumineth/Stoneguard.h"
#include "lumineth/Stonemage.h"
#include "lumineth/Cathallar.h"
#include "lumineth/SpiritOfTheMountain.h"
#include "lumineth/Teclis.h"
#include "lumineth/Avalenor.h"
#include "lumineth/MyariLightcaller.h"
#include "lumineth/MyarisPurifiers.h"

namespace LuminethRealmLords {

    std::string LuminethBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Nation") {
            auto nationName = magic_enum::enum_name((GreatNation) parameter.intValue);
            return std::string(nationName);
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

    int LuminethBase::EnumStringToInt(const std::string &enumString) {
        auto nation = magic_enum::enum_cast<GreatNation>(enumString);
        if (nation.has_value()) return (int) nation.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        return 0;
    }

    LuminethBase::LuminethBase() {
        s_globalBraveryMod.connect(this, &LuminethBase::majestic, &m_majesticConnection);
    }

    LuminethBase::~LuminethBase() {
        m_majesticConnection.disconnect();
    }

    void LuminethBase::setNation(GreatNation nation) {
        removeKeyword(SYAR);
        removeKeyword(ILIATHA);
        removeKeyword(ZAITREC);
        removeKeyword(YMETRICA);

        m_nation = nation;
        switch (nation) {
            case GreatNation::Syar:
                addKeyword(SYAR);
                break;
            case GreatNation::Iliatha:
                addKeyword(ILIATHA);
                break;
            case GreatNation::Zaitrec:
                addKeyword(ZAITREC);
                break;
            case GreatNation::Ymetrica:
                addKeyword(MORPHANN);
                break;
            default:
                break;
        }
    }

    void LuminethBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void LuminethBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    int LuminethBase::braveryModifier() const {
        auto mod = Unit::braveryModifier();

        // Aetherquartz Reserve
        if (m_aetherQuartzReserve == 0) mod--;

        // Soul-bond
        if (m_nation == GreatNation::Iliatha) {
            if (hasKeyword(VANARI) || hasKeyword(AELEMENTIRI)) {
                mod += 2;
            }
        }
        return mod;
    }

    int LuminethBase::woundModifier() const {
        auto mod = Unit::woundModifier();

        // Enduring
        if (m_commandTrait == CommandTrait::Enduring) mod += 3;

        return mod;
    }

    int LuminethBase::majestic(const Unit *unit) {
        auto mod = 0;
        if (m_commandTrait == CommandTrait::Majestic) {
            if (isFriendly(unit)) {
                if (distanceTo(unit) <= 12.0f) mod++;
            } else {
                if (distanceTo(unit) <= 18.0f) mod--;
            }
        }
        return mod;
    }

    void LuminethBase::onRestore() {
        Unit::onRestore();

        m_aetherQuartzReserve = 1;

        // Gleaming Brightness
        if (m_nation == GreatNation::Syar) {
            m_aetherQuartzReserve = 2;
        }
    }

    int LuminethBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        return Unit::toHitModifier(weapon, target);
    }

    int LuminethBase::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        return Unit::toSaveModifier(weapon, attacker);
    }

    int LuminethBase::castingModifier() const {
        return Unit::castingModifier();
    }

    void Init() {
        AlarithSpiritOfTheMountain::Init();
        AlarithStoneguard::Init();
        AlarithStonemage::Init();
        ArchmageTeclis::Init();
        AuralanWardens::Init();
        AuralanSentinels::Init();
        AvalenorTheStoneheartKing::Init();
        Dawnriders::Init();
        TheLightOfEltharion::Init();
        ScinariCathallar::Init();
        MyariLigthcaller::Init();
        MyarisPurifiers::Init();
    }

} //namespace LuminethRealmLords
