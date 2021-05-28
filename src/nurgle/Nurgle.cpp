/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <magic_enum.hpp>
#include <nurgle/Nurgle.h>
#include <Board.h>
#include <Roster.h>

#include "nurgle/BeastsOfNurgle.h"
#include "nurgle/BloabRotspawned.h"
#include "nurgle/Epidemius.h"
#include "nurgle/FeculaFlyblown.h"
#include "nurgle/Festus.h"
#include "nurgle/GreatUncleanOne.h"
#include "nurgle/Gutrot.h"
#include "nurgle/HarbingerOfDecay.h"
#include "nurgle/HorticulousSlimux.h"
#include "nurgle/LordOfAfflictions.h"
#include "nurgle/LordOfBlights.h"
#include "nurgle/LordOfPlagues.h"
#include "nurgle/MorbidexTwiceborn.h"
#include "nurgle/OrghottsDaemonspew.h"
#include "nurgle/Plaguebearers.h"
#include "nurgle/PlagueDrones.h"
#include "nurgle/Poxbringer.h"
#include "nurgle/PusgoyleBlightlords.h"
#include "nurgle/PutridBlightkings.h"
#include "nurgle/Rotigus.h"
#include "nurgle/SloppityBilepiper.h"
#include "nurgle/SpoilpoxScrivener.h"
#include "nurgle/TheGlottkin.h"
#include "nurgle/TheWurmspat.h"
#include "nurgle/Nurglings.h"
#include "nurgle/SorcererOfNurgle.h"

namespace Nurgle {

    CycleOfCorruption NurgleBase::s_currentCorruption = CycleOfCorruption::Corrupted_Regrowth;
    bool NurgleBase::s_usedGrandfathersBlessing = false;

    std::string NurgleBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Plague Legion") {
            auto legionName = magic_enum::enum_name((PlagueLegion) parameter.intValue);
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

    int NurgleBase::EnumStringToInt(const std::string &enumString) {
        auto legion = magic_enum::enum_cast<PlagueLegion>(enumString);
        if (legion.has_value()) return (int) legion.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        return 0;
    }

    void NurgleBase::setLegion(PlagueLegion legion) {
        removeKeyword(MUNIFICENT_WANDERERS);
        removeKeyword(DRONING_GUARD);
        removeKeyword(BLESSED_SONS);
        removeKeyword(DROWNED_MEN);

        m_plagueLegion = legion;
        switch (m_plagueLegion) {
            case PlagueLegion::Munificent_Wanderers:
                addKeyword(MUNIFICENT_WANDERERS);
                break;
            case PlagueLegion::Drowning_Guard:
                addKeyword(DRONING_GUARD);
                break;
            case PlagueLegion::Blessed_Sons:
                addKeyword(BLESSED_SONS);
                break;
            case PlagueLegion::Drowned_Men:
                addKeyword(DROWNED_MEN);
                break;
            default:
                break;
        }
    }

    void NurgleBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if ((owningPlayer() == player) && (s_currentCorruption == CycleOfCorruption::Corrupted_Regrowth)) {
            heal(Dice::RollD3());
        }

        if ((owningPlayer() == player) && isGeneral() && (m_commandTrait == CommandTrait::Living_Plague)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 4) {
                    unit->applyDamage({0, 1, Wounds::Source::Ability}, this);
                    getRoster()->incrementResource(1);
                }
            }
        }
    }

    int NurgleBase::moveModifier() const {
        auto mod = Unit::moveModifier();
        if (s_currentCorruption == CycleOfCorruption::Unnatural_Vitality) {
            mod += 2;
        }
        return mod;
    }

    int NurgleBase::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        if ((s_currentCorruption == CycleOfCorruption::Fecund_Vigour) && (weapon->isMelee())) {
            mod++;
        }
        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Hulking_Physique)) {
            mod++;
        }
        return mod;
    }

    int NurgleBase::runModifier() const {
        auto mod = Unit::runModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Avalanch_Of_Rotten_Flesh)) {
            mod += 2;
        }
        return mod;
    }

    int NurgleBase::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Avalanch_Of_Rotten_Flesh)) {
            mod += 2;
        }
        return mod;
    }

    int NurgleBase::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Unit::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Virulent_Contagion)) {
            rend--;
        }
        return rend;
    }

    Wounds NurgleBase::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;
        if (isGeneral() && (m_commandTrait == CommandTrait::Resilient)) {
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 6, numSixes);
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    void Init() {
        LordOfPlagues::Init();
        Plaguebearers::Init();
        PutridBlightkings::Init();
        BeastsOfNurgle::Init();
        TheGlottkin::Init();
        PlagueDrones::Init();
        PusgoyleBlightlords::Init();
        Rotigus::Init();
        GreatUncleanOne::Init();
        FeculaFlyblown::Init();
        TheWurmspat::Init();
        BloabRotspawned::Init();
        EpidemiusTallymanOfNurgle::Init();
        FestusTheLeechlord::Init();
        GutrotSpume::Init();
        HarbingerOfDecay::Init();
        HorticulousSlimux::Init();
        LordOfAfflictions::Init();
        LordOfBlights::Init();
        OrghottsDaemonspew::Init();
        PoxbringerHeraldOfNurgle::Init();
        SloppityBilepiperHeraldOfNurgle::Init();
        SpoilpoxScrivenerHeraldOfNurgle::Init();
        Nurglings::Init();
        SorcererOfNurgle::Init();
        MorbidexTwiceborn::Init();
    }

} //namespace Nurgle
