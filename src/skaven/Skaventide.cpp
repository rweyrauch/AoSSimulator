/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Skaventide.h>
#include <UnitFactory.h>
#include <magic_enum.hpp>
#include <Roster.h>
#include <Board.h>

#include "skaven/ArchWarlock.h"
#include "skaven/Clanrats.h"
#include "skaven/Clawlord.h"
#include "skaven/Deathmaster.h"
#include "skaven/Doomflayer.h"
#include "skaven/Doomwheel.h"
#include "skaven/GiantRats.h"
#include "skaven/GreySeer.h"
#include "skaven/GreySeerScreamingBell.h"
#include "skaven/GutterRunners.h"
#include "skaven/HellPitAbomination.h"
#include "skaven/LordSkreech.h"
#include "skaven/MasterMoulder.h"
#include "skaven/NightRunners.h"
#include "skaven/Packmasters.h"
#include "skaven/PlagueCenserBearers.h"
#include "skaven/Plagueclaw.h"
#include "skaven/PlagueMonks.h"
#include "skaven/PlaguePriestFurnace.h"
#include "skaven/PlaguePriest.h"
#include "skaven/RatlingGun.h"
#include "skaven/RatOgors.h"
#include "skaven/RatSwarms.h"
#include "skaven/SkryeAcolytes.h"
#include "skaven/Stormfiends.h"
#include "skaven/Stormvermin.h"
#include "skaven/ThanquolBoneripper.h"
#include "skaven/VerminlordCorruptor.h"
#include "skaven/VerminlordDeceiver.h"
#include "skaven/VerminlordWarbringer.h"
#include "skaven/VerminlordWarpseer.h"
#include "skaven/WarlockBombardier.h"
#include "skaven/WarlockEngineer.h"
#include "skaven/WarpfireThrower.h"
#include "skaven/WarpGrinder.h"
#include "skaven/WarpLightningCannon.h"
#include "skaven/WarplockJezzails.h"
#include "skaven/SkritchSpiteclaw.h"
#include "skaven/SpiteclawsSwarm.h"

namespace Skaven {

    int Skaventide::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toHitModifier(weapon, target);

        // Overwhelming Mass
        if (!weapon->isMissile() && remainingModels() >= 20)
            modifier += 1;

        return modifier;
    }

    int Skaventide::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toWoundModifier(weapon, target);

        // Overwhelming Mass
        if (!weapon->isMissile() && remainingModels() >= 30)
            modifier += 1;

        return modifier;
    }

    int Skaventide::braveryModifier() const {
        int modifier = Unit::braveryModifier();

        // Strength in Numbers (add an additional +1 for each 10 models)
        modifier += remainingModels() / 10;

        auto general = dynamic_cast<Skaventide *>(getRoster()->getGeneral());
        if (general && (general->m_commandTrait == CommandTrait::Savage_Overlord) && (distanceTo(general) < 18.0)) {
            modifier++;
        }
        return modifier;
    }

    bool Skaventide::moreMore() const {
        return (Dice::RollD6() >= 3);
    }

    std::string Skaventide::ValueToString(const Parameter &parameter) {
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

    int Skaventide::EnumStringToInt(const std::string &enumString) {
        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        return 0;
    }

    void Skaventide::onCommandAbilityUsed(const CommandAbility *ability, const Unit *target) {
        Unit::onCommandAbilityUsed(ability, target);

        // Skilled Manipulators
        if (hasKeyword(MASTERCLAN) && (Dice::RollD6() >= 5)) {
            getRoster()->addCommandPoints(1);
        }
    }

    Rerolls Skaventide::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Malevolent) && weapon->isMelee()) {
            return Rerolls::Ones;
        }

        if (hasKeyword(CLANS_PESTILENS)) {
            auto general = dynamic_cast<Skaventide *>(getRoster()->getGeneral());
            if (general && (general->remainingModels() > 0) &&
                (general->m_commandTrait == CommandTrait::Architect_Of_Death) &&
                distanceTo(general) < 18.0) {
                return Rerolls::Failed;
            }
        }

        return Unit::toWoundRerolls(weapon, target);
    }

    void Skaventide::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
        if (m_commandTrait == CommandTrait::Incredible_Agility) {
            m_fly = true;
        }
    }

    int Skaventide::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Burly)) {
            mod++;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Powerful)) {
            mod++;
        }
        return mod;
    }

    Wounds Skaventide::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;
        if (isGeneral() && (m_commandTrait == CommandTrait::Verminous_Valour)) {
            auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), SKAVENTIDE, 3.0);
            if (unit) {
                totalWounds = ignoreWounds(wounds, 4);
                auto unitWounds = wounds;
                unitWounds -= totalWounds;
                unitWounds.source = Wounds::Source::Ability;
                unit->applyDamage(unitWounds, this);
            }
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    int Skaventide::castingModifier() const {
        auto mod = Unit::castingModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Master_Of_Magic) &&
            (m_usedMasterOfMagicInRound != m_battleRound)) {
            m_usedMasterOfMagicInRound = m_battleRound;
            mod++;
        }
        return mod;
    }

    int Skaventide::unbindingModifier() const {
        auto mod = Unit::unbindingModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Master_Of_Magic) &&
            (m_usedMasterOfMagicInRound != m_battleRound)) {
            m_usedMasterOfMagicInRound = m_battleRound;
            mod++;
        }
        return mod;
    }

    void Skaventide::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (owningPlayer() == player) {
            if (isGeneral() && (m_commandTrait == CommandTrait::Deranged_Inventor)) {
                auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), CLANS_SKRYRE, 13.0);
                if (unit) {
                    unit->buffReroll(Attribute::To_Hit_Missile, Rerolls::Failed,
                                     {Phase::Shooting, m_battleRound, owningPlayer()});
                }
            }
            if (isGeneral() && (m_commandTrait == CommandTrait::Overseer_Of_Destruction)) {
                auto units = Board::Instance()->getUnitsWithKeyword(owningPlayer(), WEAPON_TEAM);
                int numBuffed = 0;
                for (auto unit : units) {
                    if (distanceTo(unit) < 13.0) {
                        unit->buffReroll(Attribute::To_Hit_Missile, Rerolls::Failed,
                                         {Phase::Shooting, m_battleRound, owningPlayer()});
                        numBuffed++;
                    }
                    if (numBuffed > 3) break;
                }
            }
        }
    }

    void Skaventide::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            if (isGeneral() && (m_commandTrait == CommandTrait::Diseased)) {
                auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
                for (auto unit : units) {
                    if (Dice::RollD6() >= 4) {
                        unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                    }
                }
            }
        }
    }

    void Init() {
        ArchWarlock::Init();
        Clanrats::Init();
        Clawlord::Init();
        Deathmaster::Init();
        Doomflayer::Init();
        Doomwheel::Init();
        GiantRats::Init();
        GreySeer::Init();
        GreySeerOnScreamingBell::Init();
        GutterRunners::Init();
        HellPitAbomination::Init();
        LordSkreechVerminking::Init();
        MasterMoulder::Init();
        NightRunners::Init();
        Packmasters::Init();
        PlagueCenserBearers::Init();
        Plagueclaw::Init();
        PlagueMonks::Init();
        PlaguePriestOnPlagueFurnace::Init();
        PlaguePriest::Init();
        RatlingGun::Init();
        RatOgors::Init();
        RatSwarms::Init();
        SkritchSpiteclaw::Init();
        SkryeAcolytes::Init();
        SpiteclawsSwarm::Init();
        Stormfiends::Init();
        Stormvermin::Init();
        ThanquolOnBoneripper::Init();
        VerminlordCorruptor::Init();
        VerminlordDeceiver::Init();
        VerminlordWarbringer::Init();
        VerminlordWarpseer::Init();
        WarlockBombardier::Init();
        WarlockEngineer::Init();
        WarpfireThrower::Init();
        WarpGrinder::Init();
        WarpLightningCannon::Init();
        WarplockJezzails::Init();
    }

} // namespace Skaven