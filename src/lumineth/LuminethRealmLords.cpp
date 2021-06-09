/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/LuminethRealmLords.h>
#include <magic_enum.hpp>
#include <Board.h>
#include "Roster.h"
#include "lumineth/AuralanSentinels.h"
#include "lumineth/AuralanWardens.h"
#include "lumineth/Avalenor.h"
#include "lumineth/Bannerblade.h"
#include "lumineth/Bladelords.h"
#include "lumineth/Calligrave.h"
#include "lumineth/Cathallar.h"
#include "lumineth/Dawnriders.h"
#include "lumineth/EllaniaAndEllathor.h"
#include "lumineth/Eltharion.h"
#include "lumineth/HurakanWindchargers.h"
#include "lumineth/LordRegent.h"
#include "lumineth/Loreseeker.h"
#include "lumineth/LyriorUthralle.h"
#include "lumineth/MyariLightcaller.h"
#include "lumineth/MyarisPurifiers.h"
#include "lumineth/Sevireth.h"
#include "lumineth/SpiritOfTheMountain.h"
#include "lumineth/SpiritOfTheWind.h"
#include "lumineth/StarshardBallista.h"
#include "lumineth/Stoneguard.h"
#include "lumineth/Stonemage.h"
#include "lumineth/Teclis.h"
#include "lumineth/Windmage.h"
#include "LuminethPrivate.h"

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

    LuminethBase::LuminethBase(GreatNation nation, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
            Unit(name, move, wounds, bravery, save, fly, points) {
        setNation(nation);
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

        if (m_commandTrait == CommandTrait::Loremaster) {
            if (hasKeyword(SCINARI)) {
                constexpr std::array<Lore, 6> loreOfHysh = {Lore::Speed_Of_Hysh, Lore::Solar_Flare, Lore::Lambent_Light,
                                                            Lore::Etheral_Blessing, Lore::Total_Eclipse, Lore::Protection_Of_Hysh,};
                // TODO: make sure added spells are unique
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(loreOfHysh[Dice::RollD6()], this)));
            }
            else if (hasKeyword(HURAKAN)) {
                constexpr std::array<Lore, 6> loreOfWinds = { Lore::Freezing_Squall, Lore::Howling_Gale,
                                                              Lore::Guiding_Flurries, Lore::Calming_Zephyr,
                                                              Lore::Burning_Simoom, Lore::Transporting_Vortex };
                // TODO: make sure added spells are unique
                m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(loreOfWinds[Dice::RollD6()], this)));
            }
        }
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
        m_usedLambentMystics = false;

        // Gleaming Brightness
        if (m_nation == GreatNation::Syar) {
            m_aetherQuartzReserve = 2;
        }
    }

    int LuminethBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        if (m_aetherQuartzReserve > 0) {
            // Heightened Senses
            mod++;
            m_aetherQuartzReserve--;
        }
        return mod;
    }

    int LuminethBase::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);
        if (m_aetherQuartzReserve > 0) {
            // Heightened Reflexes
            mod++;
            m_aetherQuartzReserve--;
        }
        return mod;
    }

    int LuminethBase::castingModifier() const {
        auto mod = Unit::castingModifier();
        if (m_aetherQuartzReserve > 0) {
            // Heightened Boost
            mod++;
            m_aetherQuartzReserve--;
        }
        if ((m_nation == GreatNation::Zaitrec) && !m_usedLambentMystics) {
            mod++;
            m_usedLambentMystics = true;
        }
        return mod;
    }

    void LuminethBase::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        if ((m_battleRound == 1) && isGeneral() && (m_commandTrait == CommandTrait::Grand_Strategist)) {
            getRoster()->addCommandPoints(1);
        }
    }

    void LuminethBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_usedLambentMystics = false;

        if (owningPlayer() == player) {
            if (isGeneral() && (m_commandTrait == CommandTrait::Warmaster) && (Dice::RollD6() >= 4)) {
                getRoster()->addCommandPoints(1);
            }
        }
    }

    int LuminethBase::moveModifier() const {
        auto mod = Unit::moveModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Swift)) {
            mod += 3;
        }
        return mod;
    }

    void LuminethBase::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (isGeneral() && (m_commandTrait == CommandTrait::Burning_Gaze)) {
            auto enemy = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (enemy && (distanceTo(enemy) < 3.0) && (enemy->remainingModels() > 0)) {
                if (Dice::RollD6() >= 2) {
                    enemy->applyDamage({0, 1, Wounds::Source::Ability}, this);
                }
            }
        }
    }

    int LuminethBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        // Gale of Killing Shafts
        if ((m_nation == GreatNation::Helon) && weapon->isMissile()) {
            auto enemy = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (enemy && (distanceTo(enemy) < 3.0)) {
                attacks++;
            }
        }
        return attacks;
    }

    int LuminethBase::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();
        if ((m_nation == GreatNation::Zaitrec) && !m_usedLambentMystics) {
            mod++;
            m_usedLambentMystics = true;
        }
        return mod;
    }

    void Init() {
        AlarithSpiritOfTheMountain::Init();
        AlarithStoneguard::Init();
        AlarithStonemage::Init();
        ArchmageTeclis::Init();
        AuralanSentinels::Init();
        AuralanWardens::Init();
        AvalenorTheStoneheartKing::Init();
        Dawnriders::Init();
        EllaniaAndEllathor::Init();
        HurakanSpiritOfTheWind::Init();
        HurakanWindchargers::Init();
        HurakanWindmage::Init();
        LyriorUthralle::Init();
        MyariLigthcaller::Init();
        MyarisPurifiers::Init();
        ScinariCalligrave::Init();
        ScinariCathallar::Init();
        ScinariLoreseeker::Init();
        Sevireth::Init();
        TheLightOfEltharion::Init();
        VanariBannerblade::Init();
        VanariBladelords::Init();
        VanariLordRegent::Init();
        VanariStarshardBallistas::Init();
    }

} //namespace LuminethRealmLords
