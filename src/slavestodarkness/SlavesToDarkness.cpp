/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <magic_enum.hpp>
#include <slavestodarkness/SlavesToDarkness.h>
#include <Board.h>
#include <Roster.h>

#include "slavestodarkness/ChaosChosen.h"
#include "slavestodarkness/ChaosKnights.h"
#include "slavestodarkness/ChaosMarauders.h"
#include "slavestodarkness/ChaosMarauderHorsemen.h"
#include "slavestodarkness/ChaosWarriors.h"
#include "slavestodarkness/CorvusCabal.h"
#include "slavestodarkness/CypherLords.h"
#include "slavestodarkness/Furies.h"
#include "slavestodarkness/IronGolems.h"
#include "slavestodarkness/Raptoryx.h"
#include "slavestodarkness/Slambo.h"
#include "slavestodarkness/SplinteredFang.h"
#include "slavestodarkness/TheUnmade.h"
#include "slavestodarkness/UntamedBeasts.h"
#include "slavestodarkness/ChaosLordOnKarkadrak.h"
#include "slavestodarkness/ChaosLordOnManticore.h"
#include "slavestodarkness/ChaosWarshrine.h"
#include "slavestodarkness/ChaosSorcererOnManticore.h"
#include "slavestodarkness/ChaosSorcerer.h"
#include "slavestodarkness/DarkoathChieftain.h"
#include "slavestodarkness/DarkoathWarqueen.h"
#include "slavestodarkness/ExaltedHeroOfChaos.h"
#include "slavestodarkness/ChaosLordOnDaemonicMount.h"
#include "slavestodarkness/ChaosLord.h"
#include "slavestodarkness/ChaosChariots.h"
#include "slavestodarkness/GorebeastChariots.h"
#include "slavestodarkness/SpireTyrants.h"
#include "slavestodarkness/MindstealerSphiranx.h"
#include "slavestodarkness/FomoroidCrusher.h"
#include "slavestodarkness/OgroidMyrmidon.h"
#include "slavestodarkness/Belakor.h"
#include "slavestodarkness/DaemonPrince.h"
#include "slavestodarkness/GodswornHunt.h"
#include "slavestodarkness/Slaughterbrute.h"
#include "slavestodarkness/SoulGrinder.h"
#include "slavestodarkness/TheddraSkullScryer.h"
#include "slavestodarkness/VortexBeast.h"
#include "slavestodarkness/ScionsOfTheFlame.h"
#include "slavestodarkness/KhagrasRavagers.h"

#include "everchosen/Archaon.h"
#include "everchosen/Varanguard.h"


namespace SlavesToDarkness {

    std::string SlavesToDarknessBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Damned Legion") {
            auto legionName = magic_enum::enum_name((DamnedLegion) parameter.intValue);
            return std::string(legionName);
        }
        if (std::string(parameter.name) == "Mark of Chaos") {
            auto markName = magic_enum::enum_name((MarkOfChaos) parameter.intValue);
            return std::string(markName);
        }
        if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore) parameter.intValue);
            return std::string(loreName);
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

    int SlavesToDarknessBase::EnumStringToInt(const std::string &enumString) {
        auto legion = magic_enum::enum_cast<DamnedLegion>(enumString);
        if (legion.has_value()) return (int) legion.value();

        auto mark = magic_enum::enum_cast<MarkOfChaos>(enumString);
        if (mark.has_value()) return (int) mark.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int) lore.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        return 0;
    }

    SlavesToDarknessBase::SlavesToDarknessBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
            Unit(name, move, wounds, bravery, save, fly) {
        s_globalBraveryMod.connect(this, &SlavesToDarknessBase::lordOfTerror, &m_lordOfTerrorSlot);
    }

    SlavesToDarknessBase::~SlavesToDarknessBase() {
        m_lordOfTerrorSlot.disconnect();
    }

    void SlavesToDarknessBase::setDamnedLegion(DamnedLegion legion) {
        removeKeyword(RAVAGERS);
        removeKeyword(CABALISTS);
        removeKeyword(DESPOILERS);
        removeKeyword(HOST_OF_THE_EVERCHOSEN);
        removeKeyword(IDOLATORS);

        m_legion = legion;
        switch (legion) {
            case DamnedLegion::Ravagers:
                addKeyword(RAVAGERS);
                break;
            case DamnedLegion::Cabalists:
                addKeyword(CABALISTS);
                break;
            case DamnedLegion::Despoilers:
                addKeyword(DESPOILERS);
                break;
            case DamnedLegion::Host_Of_The_Everchosen:
                addKeyword(HOST_OF_THE_EVERCHOSEN);
                break;
            case DamnedLegion::Idolators:
                addKeyword(IDOLATORS);
                break;
            default:
                break;
        }
    }

    void SlavesToDarknessBase::setMarkOfChaos(MarkOfChaos mark) {
        removeKeyword(UNDIVIDED);
        removeKeyword(NURGLE);
        removeKeyword(KHORNE);
        removeKeyword(SLAANESH);
        removeKeyword(TZEENTCH);

        m_markOfChaos = mark;

        switch (mark) {
            case MarkOfChaos::Undivided:
                addKeyword(UNDIVIDED);
                break;
            case MarkOfChaos::Nurgle:
                addKeyword(NURGLE);
                break;
            case MarkOfChaos::Khorne:
                addKeyword(KHORNE);
                break;
            case MarkOfChaos::Slaanesh:
                addKeyword(SLAANESH);
                break;
            case MarkOfChaos::Tzeentch:
                addKeyword(TZEENTCH);
                break;
        }
    }

    Rerolls SlavesToDarknessBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Aura of Khorne
        if (hasKeyword(KHORNE)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(KHORNE)) {
                return Rerolls::Ones;
            }
        }

        if (isGeneral() && target->hasKeyword(ORDER) && (m_commandTrait == CommandTrait::Eternal_Vendetta)) {
            return Rerolls::Failed;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int SlavesToDarknessBase::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);

        // Aura of Khorne
        if (hasKeyword(KHORNE)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(KHORNE) && hero->isGeneral()) {
                mod++;
            }
        }

        return mod;
    }

    Rerolls SlavesToDarknessBase::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Aura of Tzeentch
        if (hasKeyword(TZEENTCH)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(TZEENTCH)) {
                return Rerolls::Ones;
            }
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    void SlavesToDarknessBase::onFriendlyUnitSlain(const Unit *attacker) {
        Unit::onFriendlyUnitSlain(nullptr);

        if (hasKeyword(KHORNE)) {
            // Add a Blood Tithe point
            getRoster()->incrementResource(1);
        }
    }

    void SlavesToDarknessBase::onEnemyUnitSlain(const Unit *unit) {
        Unit::onEnemyUnitSlain(unit);

        if (hasKeyword(KHORNE)) {
            // Add a Blood Tithe point
            getRoster()->incrementResource(1);
        }
    }

    Wounds SlavesToDarknessBase::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Unit::applyWoundSave(wounds, attackingUnit);
        // Aura of Tzeentch
        if (hasKeyword(TZEENTCH)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(TZEENTCH)) {
                totalWounds.mortal = 0;
            }
        }
        // Aura of Chaos Undivided
        if (hasKeyword(UNDIVIDED)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(UNDIVIDED)) {
                totalWounds = ignoreWounds(totalWounds, 6);
            }
        }

        if (m_haveFlamesOfChaos) {
            if (Dice::RollD6() >= 4) {
                totalWounds.mortal = 0;
            }
        }

        if (m_haveUnholyResilience) {
            totalWounds = ignoreWounds(totalWounds, 5);
        }

        return totalWounds;
    }

    Wounds
    SlavesToDarknessBase::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);

        // Aura of Nurgle
        if (hasKeyword(NURGLE) && (hitRoll == 6)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(NURGLE)) {
                damage.normal++;
            }
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Flames_Of_Spite) && (hitRoll == 6)) {
            damage.mortal++;
        }
        return damage;
    }

    int SlavesToDarknessBase::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int mod = Unit::targetHitModifier(weapon, attacker);

        // Aura of Nurgle
        if (hasKeyword(NURGLE) && weapon->isMissile()) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(NURGLE) && hero->isGeneral()) {
                mod--;
            }
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Master_Of_Deception) && weapon->isMelee()) {
            mod--;
        }
        if (isGeneral() && weapon->isMissile() && (m_commandTrait == CommandTrait::Lightning_Reflexes)) {
            mod--;
        }
        return mod;
    }

    int SlavesToDarknessBase::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        int extra = Unit::generateHits(unmodifiedHitRoll, weapon, unit);

        // Aura of Slaanesh
        if (hasKeyword(SLAANESH) && (unmodifiedHitRoll == 6)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(SLAANESH)) {
                extra++;
            }
        }
        return extra;
    }

    Rerolls SlavesToDarknessBase::runRerolls() const {
        // Aura of Slaanesh
        if (hasKeyword(SLAANESH)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(SLAANESH) && hero->isGeneral()) {
                return Rerolls::Failed;
            }
        }
        return Unit::runRerolls();
    }

    Rerolls SlavesToDarknessBase::chargeRerolls() const {
        // Aura of Slaanesh
        if (hasKeyword(SLAANESH)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(SLAANESH) && hero->isGeneral()) {
                return Rerolls::Failed;
            }
        }
        return Unit::chargeRerolls();
    }

    void SlavesToDarknessBase::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        // Eye of the Gods
        if ((m_currentRecord.m_herosSlain > 0) || (m_currentRecord.m_monstersSlain > 0)) {
            if (hasKeyword(HERO) && hasKeyword(EYE_OF_THE_GODS) && hasKeyword(SLAVES_TO_DARKNESS)) {
                auto roll = Dice::Roll2D6();
                if ((roll >= 11) && !m_haveDarkApotheosis) {
                    // Can heal D3 wounds to this hero or replace this hero with a Daemon Prince
                    if (Dice::RollD6() > 5) {
                        heal(Dice::RollD3());
                    } else {
                        // Dark Apotheosis
                        auto factory = UnitFactory::LookupUnit("Daemon Prince");
                        if (factory) {
                            if (m_roster) {
                                auto parameters = factory->m_parameters;
                                for (auto &ip : parameters) {
                                    if ((ip.paramType == ParamType::Enum) &&
                                        (ip.name == std::string("Mark of Chaos"))) {
                                        ip.intValue = (int) m_markOfChaos;
                                    }
                                }
                                auto unit = std::shared_ptr<Unit>(UnitFactory::Create("Daemon Prince", parameters));
                                unit->deploy(position(), orientation());
                                m_roster->addUnit(unit);
                            }
                        }
                        // TODO: This model is removed from play and replaced by the Daemon Prince.  The removed
                        // hero does not count as having fled or been slain. (Add a deployed state?)
                        m_models.front()->flee();

                        // TODO: The new daemon prince has the same artefacts/spells/etc. as the hero being replaced.
                    }
                    m_haveDarkApotheosis = true;
                } else if ((roll >= 9) && !m_haveDaemonicLegions) {
                    // Daemonic Legions
                    auto unitName("");
                    auto numModels = 0;
                    switch (m_markOfChaos) {
                        case MarkOfChaos::Khorne:
                            unitName = "Bloodletters";
                            numModels = 10;
                            break;
                        case MarkOfChaos::Nurgle:
                            unitName = "Plaguebearers";
                            numModels = 10;
                            break;
                        case MarkOfChaos::Slaanesh:
                            unitName = "Daemonettes";
                            numModels = 10;
                            break;
                        case MarkOfChaos::Tzeentch:
                            unitName = "Pink Horrors";
                            numModels = 10;
                            break;
                        case MarkOfChaos::Undivided:
                            unitName = "Furies";
                            numModels = 6;
                            break;
                    }

                    if (numModels) {
                        auto factory = UnitFactory::LookupUnit(unitName);
                        if (factory) {
                            if (m_roster) {
                                auto parameters = factory->m_parameters;
                                for (auto &ip : parameters) {
                                    if ((ip.paramType == ParamType::Integer) && (ip.name == std::string("Models"))) {
                                        ip.intValue = numModels;
                                    }
                                }
                                auto unit = std::shared_ptr<Unit>(UnitFactory::Create(unitName, parameters));
                                unit->deploy(position(), orientation());
                                m_roster->addUnit(unit);
                            }
                        }
                    }
                    m_haveDaemonicLegions = true;
                } else if ((roll == 8) && !m_haveUnholyResilience) {
                    // Unholy Resilience
                    m_haveUnholyResilience = true;
                } else if (roll == 7) {
                    // Snubbed by the Gods
                } else if ((roll == 6) && !m_haveFlamesOfChaos) {
                    // Flames of Chaos
                    m_haveFlamesOfChaos = true;
                } else if ((roll == 5) && !m_haveIronFlesh) {
                    // Iron Flesh
                    m_haveIronFlesh = true;
                } else if ((roll == 4) && !m_haveMurderousMutation) {
                    // Murderous Mutation
                    m_haveMurderousMutation = true;
                } else if ((roll == 3) && !m_haveSlaughtersStrength) {
                    // Slaughter's Strength
                    m_haveSlaughtersStrength = true;
                } else if ((roll == 2) && !m_haveSpawndom) {
                    // Spawndom
                    auto factory = UnitFactory::LookupUnit("Chaos Spawn");
                    if (factory) {
                        if (m_roster) {
                            auto unit = std::shared_ptr<Unit>(
                                    UnitFactory::Create("Chaos Spawn", factory->m_parameters));
                            unit->deploy(position(), orientation());
                            m_roster->addUnit(unit);
                        }
                    }
                    m_haveSpawndom = true;
                }
            }
        }
    }

    void SlavesToDarknessBase::onRestore() {
        Unit::onRestore();

        m_haveDarkApotheosis = false;
        m_haveDaemonicLegions = false;
        m_haveUnholyResilience = false;
        m_haveFlamesOfChaos = false;
        m_haveIronFlesh = false;
        m_haveMurderousMutation = false;
        m_haveSlaughtersStrength = false;
        m_haveSpawndom = false;
    }

    int SlavesToDarknessBase::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);

        if (m_haveIronFlesh) mod++;

        return mod;
    }

    int SlavesToDarknessBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Bolstered_By_Hate)) {
            mod += 1;
        }
        return mod;
    }

    int SlavesToDarknessBase::rollChargeDistance() {
        if (hasKeyword(IDOLATORS) && hasKeyword(CULTISTS)) {
            // Panoply of Pain
            auto roll1 = Dice::RollD6();
            auto roll2 = Dice::RollD6();
            if (roll1 < roll2) roll1 = 6;
            else if (roll2 < roll1) roll2 = 6;
            else roll1 = 6;

            m_unmodifiedChargeRoll = roll1 + roll2;
            return m_unmodifiedChargeRoll + chargeModifier();
        }
        return Unit::rollChargeDistance();
    }

    bool SlavesToDarknessBase::setCommandTrait(CommandTrait trait) {
        // Verify against legion
        m_commandTrait = trait;
        return true;
    }

    bool SlavesToDarknessBase::setArtefact(Artefact artefact) {
        // Verify against legion
        m_artefact = artefact;
        return true;
    }

    Rerolls SlavesToDarknessBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Eternal_Vendetta)) {
            return Rerolls::Failed;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int SlavesToDarknessBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        if (isGeneral() && weapon->isMelee() && (m_commandTrait == CommandTrait::Smite_The_Unbeliever)) {
            attacks += 2;
        }
        return attacks;
    }

    int SlavesToDarknessBase::lordOfTerror(const Unit *unit) {
        if (m_commandTrait == CommandTrait::Lord_Of_Terror) {
            if (!isFriendly(unit) && (distanceTo(unit) < 6.0)) {
                return -1;
            }
        }
        return 0;
    }

    void SlavesToDarknessBase::onEndHero(PlayerId player) {
        Unit::onEndHero(player);

        if ((owningPlayer() == player) && (m_commandTrait == CommandTrait::Radiance_Of_Dark_Glory)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
            for (auto unit : units) {
                if (unit->numOfWoundedModels() > 0) {
                    if (Dice::RollD6() >= 3) {
                        unit->heal(Dice::RollD3());
                    }
                    break;
                }
            }
        }
    }

    void Init() {
        Archaon::Init();
        Varanguard::Init();

        Belakor::Init();
        ChaosChariots::Init();
        ChaosChosen::Init();
        ChaosKnights::Init();
        ChaosLord::Init();
        ChaosLordOnDaemonicMount::Init();
        ChaosLordOnKarkadrak::Init();
        ChaosLordOnManticore::Init();
        ChaosMarauderHorsemen::Init();
        ChaosMarauders::Init();
        ChaosSorcerer::Init();
        ChaosSorcererOnManticore::Init();
        ChaosWarriors::Init();
        ChaosWarshrine::Init();
        CorvusCabal::Init();
        CypherLords::Init();
        DaemonPrince::Init();
        DarkoathChieftain::Init();
        DarkoathWarqueen::Init();
        ExaltedHeroOfChaos::Init();
        FomoroidCrusher::Init();
        Furies::Init();
        GodswornHunt::Init();
        GorebeastChariots::Init();
        IronGolems::Init();
        KhagrasRavagers::Init();
        MindstealerSphiranx::Init();
        MutalithVortexBeast::Init();
        OgroidMyrmidon::Init();
        Raptoryx::Init();
        ScionsOfTheFlame::Init();
        Slambo::Init();
        Slaughterbrute::Init();
        SoulGrinder::Init();
        SpireTyrants::Init();
        SplinteredFang::Init();
        TheddraSkullscryer::Init();
        TheUnmade::Init();
        UntamedBeasts::Init();
    }

} //namespace SlavesToDarkness
