/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Board.h>
#include <magic_enum.hpp>
#include "ossiarch/OssiarchBonereaperBase.h"

#include "ossiarch/MortekGuard.h"
#include "ossiarch/NecropolisStalkers.h"
#include "ossiarch/Vokmortian.h"
#include "ossiarch/ImmortisGuard.h"
#include "ossiarch/KavalosDeathriders.h"
#include "ossiarch/GothizzarHarvester.h"
#include "ossiarch/MortekCrawler.h"
#include "ossiarch/Katakros.h"
#include "ossiarch/LiegeKavalos.h"
#include "ossiarch/KavalosZandtos.h"
#include "ossiarch/Boneshaper.h"
#include "ossiarch/Soulmason.h"
#include "ossiarch/Soulreaper.h"
#include "ossiarch/MorghastArchai.h"
#include "ossiarch/MorghastHarbingers.h"
#include "ossiarch/ArkhanTheBlack.h"

namespace OssiarchBonereapers {

    std::string OssiarchBonereaperBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Legion") {
            auto legionName = magic_enum::enum_name((Legion) parameter.intValue);
            return std::string(legionName);
        } else if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        } else if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        } else if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore) parameter.intValue);
            return std::string(loreName);
        }
        return ParameterValueToString(parameter);
    }

    int OssiarchBonereaperBase::EnumStringToInt(const std::string &enumString) {
        auto legion = magic_enum::enum_cast<Legion>(enumString);
        if (legion.has_value()) return (int) legion.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        return 0;
    }

    OssiarchBonereaperBase::OssiarchBonereaperBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
            Unit(name, move, wounds, bravery, save, fly) {
    }

    OssiarchBonereaperBase::~OssiarchBonereaperBase() {
        m_braveryBuffConnection.disconnect();
        m_direUltimatumSlot.disconnect();
    }

    void OssiarchBonereaperBase::setLegion(Legion legion) {
        removeKeyword(MORTIS_PRAETORIANS);
        removeKeyword(PETRIFEX_ELITE);
        removeKeyword(STALLIARCH_LORDS);
        removeKeyword(IVORY_HOST);
        removeKeyword(NULL_MYRIAD);
        removeKeyword(CREMATORIANS);

        m_braveryBuffConnection.disconnect();
        m_runAndCharge = false;

        m_legion = legion;
        switch (legion) {
            case Legion::Mortis_Praetorians:
                addKeyword(MORTIS_PRAETORIANS);
                // The Dread Legion
                s_globalBraveryMod.connect(this, &OssiarchBonereaperBase::theDreadLegion, &m_braveryBuffConnection);
                break;
            case Legion::Petrifex_Elite:
                addKeyword(PETRIFEX_ELITE);
                break;
            case Legion::Stalliarch_Lords:
                // Equumortoi
                m_runAndCharge = true;
                addKeyword(STALLIARCH_LORDS);
                break;
            case Legion::Ivory_Host:
                addKeyword(IVORY_HOST);
                break;
            case Legion::Null_Myriad:
                addKeyword(NULL_MYRIAD);
                break;
            case Legion::Crematorians:
                addKeyword(CREMATORIANS);
                break;
            default:
                break;
        }

    }

    Wounds OssiarchBonereaperBase::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Deathless Warriors
        auto hekatos = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HEKATOS, 6.0);
        auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 16.0);

        Wounds totalWounds = wounds;

        if (hasKeyword(HEKATOS) || hekatos || hero) {
            totalWounds = ignoreWounds(totalWounds, 6);
        }
        return totalWounds;
    }

    void OssiarchBonereaperBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
        m_direUltimatumSlot.disconnect();
        if (m_commandTrait == CommandTrait::Dire_Ultimatum) {
            s_globalBraveryMod.connect(this, &OssiarchBonereaperBase::direUltimatum, &m_direUltimatumSlot);
        }
    }

    void OssiarchBonereaperBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    Rerolls OssiarchBonereaperBase::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Unstoppable Juggernauts
        if (m_legion == Legion::Petrifex_Elite) {
            return Rerolls::Ones;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int OssiarchBonereaperBase::theDreadLegion(const Unit *target) {
        // The Dread Legion
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 12.0)) {
            return -1;
        }
        return 0;
    }

    void OssiarchBonereaperBase::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        Unit::onFriendlyModelSlain(numSlain, attacker, source);

        // Immoliate
        if ((source == Wounds::Source::Weapon_Melee) && (m_legion == Legion::Crematorians)) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0f)) {
                auto roll = Dice::RollD6();
                if (hasKeyword(HERO) || hasKeyword(MONSTER)) roll++;
                if (roll >= 5) {
                    unit->applyDamage({0, 1}, this);
                }
            }
        }
    }

    bool OssiarchBonereaperBase::battleshockRequired() const {
        // Ranks Unbroken by Dissent
        return false;
    }

    int OssiarchBonereaperBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (m_commandTrait == CommandTrait::Mighty_Archaeossian) {
            mod += 2;
        }
        return mod;
    }

    Wounds OssiarchBonereaperBase::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((m_commandTrait == CommandTrait::Peerless_Warrior) && weapon->isMelee() && (woundRoll == 6)) {
            return {weapon->damage(), 1, Wounds::Source::Weapon_Melee};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int OssiarchBonereaperBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        if (weapon->isMelee() && (m_commandTrait == CommandTrait::Hatred_Of_The_Living) && !target->hasKeyword(DEATH)) {
            mod++;
        }
        return mod;
    }

    int OssiarchBonereaperBase::direUltimatum(const Unit *target) {
        if ((target->owningPlayer() != owningPlayer()) && (m_commandTrait == CommandTrait::Dire_Ultimatum) && (distanceTo(target) <= 12.0)) {
            return -1;
        }
        return 0;
    }

    void Init() {
        ArchKavalosZandtos::Init();
        ArkhanTheBlack::Init();
        GothizzarHarvester::Init();
        ImmortisGuard::Init();
        KavalosDeathriders::Init();
        LiegeKavalos::Init();
        MorghastArchai::Init();
        MorghastHarbingers::Init();
        MortekCrawler::Init();
        MortekGuard::Init();
        MortisanBoneshaper::Init();
        MortisanSoulmason::Init();
        MortisanSoulreaper::Init();
        NecropolisStalkers::Init();
        Katakros::Init();
        Vokmortian::Init();
    }

} // namespace OssiarchBonereapers
