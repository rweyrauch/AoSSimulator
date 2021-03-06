/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <sylvaneth/SylvanethBase.h>
#include <Board.h>
#include <Roster.h>
#include <magic_enum.hpp>
#include <sylvaneth/SylvanethSpells.h>
#include <sylvaneth/AwakenedWyldwood.h>

#include "sylvaneth/Alarielle.h"
#include "sylvaneth/ArchRevenant.h"
#include "sylvaneth/Branchwraith.h"
#include "sylvaneth/Branchwych.h"
#include "sylvaneth/Dryads.h"
#include "sylvaneth/DrychaHamadreth.h"
#include "sylvaneth/KurnothHunters.h"
#include "sylvaneth/SkaethsWildHunt.h"
#include "sylvaneth/SpiritOfDurthu.h"
#include "sylvaneth/SpiteRevenants.h"
#include "sylvaneth/TreeRevenants.h"
#include "sylvaneth/Treelord.h"
#include "sylvaneth/TreelordAncient.h"
#include "sylvaneth/WarsongRevenant.h"
#include "sylvaneth/Ylthari.h"
#include "sylvaneth/YlthariGardians.h"


namespace Sylvaneth {

    void SylvanethBase::setGlade(Glade glade) {
        removeKeyword(OAKENBROW);
        removeKeyword(GNARLROOT);
        removeKeyword(HEARTWOOD);
        removeKeyword(IRONBARK);
        removeKeyword(WINTERLEAF);
        removeKeyword(DREADWOOD);
        removeKeyword(HARVESTBOON);

        m_glade = glade;
        switch (m_glade) {
            case Glade::Oakenbrow:
                addKeyword(OAKENBROW);
                break;
            case Glade::Gnarlroot:
                addKeyword(GNARLROOT);
                break;
            case Glade::Heartwood:
                addKeyword(HEARTWOOD);
                break;
            case Glade::Ironbark:
                addKeyword(IRONBARK);
                break;
            case Glade::Winterleaf:
                addKeyword(WINTERLEAF);
                break;
            case Glade::Dreadwood:
                addKeyword(DREADWOOD);
                break;
            case Glade::Harvestboon:
                addKeyword(HARVESTBOON);
                break;
            default:
                break;
        }
    }

    Rerolls SylvanethBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Lord_Of_Spites)) {
            return Rerolls::Ones;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    Rerolls SylvanethBase::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Vibrant Surge
        if (hasKeyword(HARVESTBOON) && m_charged) {
            return Rerolls::Ones;
        }
            // Shield the Arcane
        else if (hasKeyword(GNARLROOT)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto ip : units) {
                if (ip->hasKeyword(GNARLROOT) && ip->hasKeyword(WIZARD)) {
                    return Rerolls::Ones;
                }
            }
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Dread_Harvester) && m_charged) {
            return Rerolls::Failed;
        }

        return Unit::toHitRerolls(weapon, unit);
    }

    std::string SylvanethBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Glade") {
            auto gladeName = magic_enum::enum_name((Glade) parameter.intValue);
            return std::string(gladeName);
        } else if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore) parameter.intValue);
            return std::string(loreName);
        } else if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        } else if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        }
        return ParameterValueToString(parameter);
    }

    int SylvanethBase::EnumStringToInt(const std::string &enumString) {
        auto glade = magic_enum::enum_cast<Glade>(enumString);
        if (glade.has_value()) return (int) glade.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        return 0;
    }

    int SylvanethBase::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Winter's Bite
        if ((unmodifiedHitRoll == 6) && hasKeyword(WINTERLEAF)) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    Rerolls SylvanethBase::battleshockRerolls() const {
        // Stubborn and Taciturn
        if (hasKeyword(IRONBARK)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto ip : units) {
                if (ip->hasKeyword(HERO) && ip->hasKeyword(IRONBARK)) {
                    return Rerolls::Failed;
                }
            }
        }
        return Unit::battleshockRerolls();
    }

    int SylvanethBase::braveryModifier() const {
        auto mod = Unit::braveryModifier();

        // Courage for Kurnoth
        if (hasKeyword(HEARTWOOD)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto ip : units) {
                if (ip->hasKeyword(HERO) && ip->hasKeyword(HEARTWOOD)) {
                    mod++;
                    break;
                }
            }
        }

        auto general = dynamic_cast<SylvanethBase *>(getRoster()->getGeneral());
        if (general && (general->remainingModels() > 0) &&
            (general->m_commandTrait == CommandTrait::Wisdom_Of_The_Ancients) && (distanceTo(general) < 12.0)) {
            mod++;
        }
        return mod;
    }

    int SylvanethBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Regal_Old_Growth)) mod += 1;
        return mod;
    }

    void SylvanethBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void SylvanethBase::setCommandTrait(CommandTrait commandTrait) {
        m_commandTrait = commandTrait;

        if (m_commandTrait == CommandTrait::Voice_Of_Warding) {
            m_totalUnbinds++;
        }

        if (m_commandTrait == CommandTrait::Arcane_Bounty) {
            constexpr std::array<Lore, 6> loreOfTheDeepwood = { Lore::Throne_Of_Vines, Lore::Regrowth, Lore::The_Dwellers_Below,
                                                                Lore::Deadly_Harvest, Lore::Verdurous_Harmony, Lore::Treesong};
            // TODO: make sure added spells are unique
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(loreOfTheDeepwood[Dice::RollD6()], this)));
        }
    }

    void SylvanethBase::onCastSpell(const Spell *spell, const Unit *target) {
        Unit::onCastSpell(spell, target);

        if (isGeneral() && (m_commandTrait == CommandTrait::Nurtured_By_Magic)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
            for (auto unit : units) {
                if (unit->remainingWounds() < unit->wounds()) {
                    unit->heal(Dice::RollD3());
                    break;
                }
            }
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Mystic_Regrowth)) {
            heal(Dice::RollD3());
        }
    }

    void SylvanethBase::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        Unit::onFriendlyModelSlain(numSlain, attacker, source);

        if (isGeneral() && (m_commandTrait == CommandTrait::Legacy_Of_Valour)) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) < 1.0)) {
                auto roll = Dice::RollD6();
                Wounds wounds = {0, Dice::RollD3(), Wounds::Source::Ability};
                if (roll == 6) {
                    wounds.mortal = Dice::RollD6();
                }
                unit->applyDamage(wounds, this);
            }
        }
    }

    Rerolls SylvanethBase::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Mere_Rainfall) && weapon->isMissile()) {
            return Rerolls::Failed;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Gnarled_Warrior)) {
            return Rerolls::Ones;
        }

        return Unit::toSaveRerolls(weapon, attacker);
    }

    void SylvanethBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            if (isGeneral() && (m_commandTrait == CommandTrait::Gift_Of_Ghyran)) {
                heal(1);
            }
        }
    }

    int SylvanethBase::chargeModifier() const {
        auto mod = Unit::chargeModifier();

        auto general = dynamic_cast<SylvanethBase *>(getRoster()->getGeneral());
        if (general && (general->remainingModels() > 0) && (general->m_commandTrait == CommandTrait::Warsinger) &&
            (distanceTo(general) < 12.0)) {
            mod += 2;
        }
        return mod;
    }

    int SylvanethBase::castingModifier() const {
        auto mod = Unit::castingModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Glade_Lore)) {
            auto woods = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 6.0);
            if (woods != nullptr) {
                mod++;
            }
        }
        return mod;
    }

    Wounds SylvanethBase::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        if (isGeneral() && (m_commandTrait == CommandTrait::Radiant_Spirit) && (wounds.source == Wounds::Source::Spell)) {
            int numSixes = 0;
            return ignoreWounds(wounds, 4, numSixes);
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    void Init() {
        Alarielle::Init();
        ArchRevenant::Init();
        AwakenedWyldwood::Init();
        Branchwraith::Init();
        Branchwych::Init();
        Dryads::Init();
        DrychaHamadreth::Init();
        KurnothHunters::Init();
        SkaethsWildHunt::Init();
        SpiritOfDurthu::Init();
        SpiteRevenants::Init();
        TreeRevenants::Init();
        Treelord::Init();
        TreelordAncient::Init();
        WarsongRevenant::Init();
        Ylthari::Init();
        YltharisGuardians::Init();
    }

} //namespace Sylvaneth