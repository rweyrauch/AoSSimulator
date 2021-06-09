/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/Nighthaunt.h>
#include <Board.h>
#include <magic_enum.hpp>

#include "nighthaunt/BlackCoach.h"
#include "nighthaunt/BladegheistRevenants.h"
#include "nighthaunt/CairnWraith.h"
#include "nighthaunt/Chainghasts.h"
#include "nighthaunt/ChainraspHorde.h"
#include "nighthaunt/DreadbladeHarrow.h"
#include "nighthaunt/DreadscytheHarridans.h"
#include "nighthaunt/GlaivewraithStalkers.h"
#include "nighthaunt/GrimghastReapers.h"
#include "nighthaunt/GuardianOfSouls.h"
#include "nighthaunt/Hexwraiths.h"
#include "nighthaunt/KnightOfShrouds.h"
#include "nighthaunt/KnightOfShroudsOnSteed.h"
#include "nighthaunt/KrulghastCruciator.h"
#include "nighthaunt/KurdossValentian.h"
#include "nighthaunt/LadyOlynder.h"
#include "nighthaunt/LordExecutioner.h"
#include "nighthaunt/Mourngul.h"
#include "nighthaunt/MyrmournBanshees.h"
#include "nighthaunt/ReikenorGrimhailer.h"
#include "nighthaunt/SpiritHosts.h"
#include "nighthaunt/SpiritTorment.h"
#include "nighthaunt/TheBriarQueen.h"
#include "nighthaunt/ThornsOfTheBriarQueen.h"
#include "nighthaunt/TombBanshee.h"

namespace Nighthaunt {

    Nighthaunt::Nighthaunt(Procession procession, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
            Unit(name, move, wounds, bravery, save, fly, points) {
        setProcession(procession);
        s_globalBraveryMod.connect(this, &Nighthaunt::auraOfDread, &m_auraOfDreadSlot);
    }

    Nighthaunt::~Nighthaunt() {
        m_auraOfDreadSlot.disconnect();
    }

    Wounds Nighthaunt::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Deathless Spirits
        auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
        if (hero && hero->hasKeyword(NIGHTHAUNT)) {
            int numSixes = 0;
            return ignoreWounds(wounds, 5, numSixes);
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    int Nighthaunt::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        // Ethereal - no save modifiers allowed.
        int modifier = 0;

        // Ethereal - ignore rend by cancelling it out.
        if (weapon->rend() < 0) {
            modifier = -weapon->rend();
        }

        return modifier;
    }

    int Nighthaunt::auraOfDread(const Unit *unit) {
        if (!isFriendly(unit) && (distanceTo(unit) < 6.0)) return -1;
        return 0;
    }

    std::string Nighthaunt::ValueToString(const Parameter &parameter) {
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
        if (std::string(parameter.name) == "Procession") {
            auto processionName = magic_enum::enum_name((Procession) parameter.intValue);
            return std::string(processionName);
        }
        return ParameterValueToString(parameter);
    }

    int Nighthaunt::EnumStringToInt(const std::string &enumString) {
        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto procession = magic_enum::enum_cast<Procession>(enumString);
        if (procession.has_value()) return (int) procession.value();

        return 0;
    }

    void Nighthaunt::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void Nighthaunt::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Nighthaunt::setProcession(Procession procession) {
        removeKeyword(EMERALD_HOST);
        removeKeyword(REIKENORS_CONDEMNED);

        m_procession = procession;

        switch (procession) {
            case Procession::Emerald_Host:
                addKeyword(EMERALD_HOST);
                break;
            case Procession::Reikenors_Condemned:
                addKeyword(REIKENORS_CONDEMNED);
                break;
            default:
                break;
        }
    }

    Rerolls Nighthaunt::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Hatred_Of_The_Living) &&
            !target->hasKeyword(DEATH)) {
            return Rerolls::Failed;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int Nighthaunt::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Lingering_Spirit)) {
            mod += 1;
        }
        return mod;
    }

    int Nighthaunt::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        if (isGeneral() && weapon->isMissile() && (m_commandTrait == CommandTrait::Cloaked_In_Shadow)) {
            mod--;
        }
        return mod;
    }

    void Init() {
        BlackCoach::Init();
        BladegheistRevenants::Init();
        CairnWraith::Init();
        Chainghasts::Init();
        ChainraspHorde::Init();
        DreadbladeHarrow::Init();
        DreadscytheHarridans::Init();
        GlaivewraithStalkers::Init();
        GrimghastReapers::Init();
        GuardianOfSouls::Init();
        Hexwraiths::Init();
        KnightOfShrouds::Init();
        KnightOfShroudsOnEtherealSteed::Init();
        KrulghastCruciator::Init();
        KurdossValentian::Init();
        LadyOlynder::Init();
        LordExecutioner::Init();
        Mourngul::Init();
        MyrmournBanshees::Init();
        ReikenorTheGrimhailer::Init();
        SpiritHosts::Init();
        SpiritTorment::Init();
        TheBriarQueen::Init();
        ThornsOfTheBriarQueen::Init();
        TombBanshee::Init();
    }

} // namespace Nighthaunt
