/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/EngineOfTheGods.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "SeraphonPrivate.h"

namespace Seraphon {

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 260;

    struct TableEntry {
        int m_move;
        int m_hornDamage;
        int m_stompAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 5, 8, 10, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 4, 5},
                    {7, 3, 4},
                    {6, 2, 3},
                    {5, 2, 3},
                    {4, 1, 1}
            };

    bool EngineOfTheGods::s_registered = false;

    EngineOfTheGods::EngineOfTheGods(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SeraphonBase("Engine of the Gods", 8, g_wounds, 6, 4, false, g_pointsPerUnit) {
        m_keywords = {ORDER, SERAPHON, STEGADON, SKINK, MONSTER, HERO, ENGINE_OF_THE_GODS};
        m_weapons = {&m_javelins, &m_horns, &m_jaws, &m_stomps};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_jaws.setMount(true);
        m_stomps.setMount(true);
        m_horns.setMount(true);

        setWayOfTheSeraphon(way, constellation);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalBattleshockReroll.connect(this, &EngineOfTheGods::steadfastMajestyBraveryReroll, &m_steadfastSlot);
        s_globalChargeReroll.connect(this, &EngineOfTheGods::cosmicEngineChargeReroll, &m_cosmicEngineChargeSlot);
        s_globalAttackMod.connect(this, &EngineOfTheGods::cosmicEngineAttackMod, &m_cosmicEngineAttackSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_javelins);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_stomps);
        addModel(model);
    }

    EngineOfTheGods::~EngineOfTheGods() {
        m_steadfastSlot.disconnect();
        m_cosmicEngineChargeSlot.disconnect();
        m_cosmicEngineAttackSlot.disconnect();
    }

    void EngineOfTheGods::onRestore() {
        SeraphonBase::onRestore();

        // Reset table-drive attributes
        onWounded();

        m_armouredCrestAttacker = nullptr;
    }

    Unit *EngineOfTheGods::Create(const ParameterList &parameters) {
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skinkCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_vestmentsOfThePriesthood[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new EngineOfTheGods(way, constellation, trait, artefact, general);
    }

    void EngineOfTheGods::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_skinkCommandTrait[0], g_skinkCommandTrait),
                            EnumParameter("Artefact", g_vestmentsOfThePriesthood[0], g_vestmentsOfThePriesthood),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Engine of the Gods", factoryMethod);
        }
    }

    void EngineOfTheGods::onWounded() {
        SeraphonBase::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_stomps.setAttacks(g_damageTable[damageIndex].m_stompAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t EngineOfTheGods::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void EngineOfTheGods::onCharged() {
        SeraphonBase::onCharged();

        // Unstoppable Stampede
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto unit : units) {
            if (Dice::RollD6() >= 3) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    void EngineOfTheGods::onStartShooting(PlayerId player) {
        SeraphonBase::onStartShooting(player);

        if (player != owningPlayer()) return;

        // Cosmic Engine
        m_timeStoodStill = false;
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), SLANN, 12.0);

        int roll = Dice::Roll2D6();
        if (unit) {
            roll = Dice::Roll3D6();
        }
        if (roll <= 3) {
            applyDamage({0, Dice::RollD3()}, this);
        } else if (roll <= 8) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto u : units) {
                if (u->hasKeyword(SERAPHON)) {
                    u->heal(Dice::RollD3());
                }
            }
            heal(Dice::RollD3());
        } else if (roll <= 12) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 12.0);
            if (units.empty()) {
                units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 24.0);
                if (!units.empty()) {
                    units.front()->applyDamage({0, Dice::RollD3()}, this);
                }
            } else {
                for (auto u : units) {
                    u->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        } else if (roll <= 17) {
            // Setup a unit of 10 Saurus Warriors
            auto factory = UnitFactory::LookupUnit("Saurus Warriors");
            if (factory) {
                if (m_roster) {
                    auto summonnedUnit = std::shared_ptr<Unit>(
                            UnitFactory::Create("Saurus Warriors", factory->m_parameters));
                    summonnedUnit->deploy(position(), m_orientation);
                    m_roster->addUnit(summonnedUnit);
                }
            }
        } else { // roll == 18
            m_timeStoodStill = true;
        }
    }

    Rerolls EngineOfTheGods::steadfastMajestyBraveryReroll(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(SKINK) && (distanceTo(unit) <= 18.0)) return Rerolls::Failed;
        return Rerolls::None;
    }

    int EngineOfTheGods::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Rerolls EngineOfTheGods::cosmicEngineChargeReroll(const Unit *unit) {
        if (m_timeStoodStill && isFriendly(unit) && unit->hasKeyword(SERAPHON) && (distanceTo(unit) <= 24.0))
            return Rerolls::Failed;
        return Rerolls::None;
    }

    int EngineOfTheGods::cosmicEngineAttackMod(const Unit *attacker, const Model *attackingModel, const Weapon *weapon,
                                               const Unit *target) {
        if (m_timeStoodStill && isFriendly(attacker) && attacker->hasKeyword(SERAPHON) &&
            (distanceTo(attacker) <= 24.0))
            return weapon->attacks();
        return 0;
    }

    void EngineOfTheGods::onStartCombat(PlayerId player) {
        SeraphonBase::onStartCombat(player);

        m_armouredCrestAttacker = nullptr;

        // Armoured Crest
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            for (auto unit : units) {
                // Select target for Armoured Crest (select first one for now).
                m_armouredCrestAttacker = unit;
                break;
            }
        }
    }

    int EngineOfTheGods::targetSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = SeraphonBase::targetSaveModifier(weapon, attacker);

        if (attacker == m_armouredCrestAttacker) mod++;

        return mod;
    }

} //namespace Seraphon
