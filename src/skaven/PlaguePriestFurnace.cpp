/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlaguePriestFurnace.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 200;

    struct TableEntry {
        int m_move;
        int m_censerDamage;
        int m_spikesToHit;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 5, 8, 10, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {6, 4, 2},
                    {6, 3, 3},
                    {4, 2, 4},
                    {4, 1, 4},
                    {3, 0, 5}
            };


    bool PlaguePriestOnPlagueFurnace::s_registered = false;

    Unit *PlaguePriestOnPlagueFurnace::Create(const ParameterList &parameters) {
        auto unit = new PlaguePriestOnPlagueFurnace();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_pestilensCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_pestilensArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void PlaguePriestOnPlagueFurnace::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_pestilensCommandTraits[0], g_pestilensCommandTraits),
                            EnumParameter("Artefact", g_pestilensArtefacts[0], g_pestilensArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Plague Priest on Plague Furnace", factoryMethod);
        }
    }

    PlaguePriestOnPlagueFurnace::PlaguePriestOnPlagueFurnace() :
            Skaventide("Plague Priest on Plague Furnace", 6, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_censer(Weapon::Type::Melee, "Great Plague Censer", 3, 1, 2, 0, 0, 0),
            m_staff(Weapon::Type::Melee, "Warpstone-tipped Staff", 2, 1, 4, 3, -1, RAND_D3),
            m_blades(Weapon::Type::Melee, "Foetid Blades", 1, 6, 4, 4, 0, 1),
            m_spikes(Weapon::Type::Melee, "Rusty Spikes", 1, RAND_D6, 2, 3, -1, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, WAR_MACHINE, HERO, PRIEST,
                      PLAGUE_FURNACE, PLAGUE_PRIEST};
        m_weapons = {&m_censer, &m_staff, &m_blades, &m_spikes};
        m_battleFieldRole = Role::Leader_Behemoth;

        s_globalBraveryMod.connect(this, &PlaguePriestOnPlagueFurnace::altarOfTheHornedRat, &m_connection);
    }

    PlaguePriestOnPlagueFurnace::~PlaguePriestOnPlagueFurnace() {
        m_connection.disconnect();
    }

    void PlaguePriestOnPlagueFurnace::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_censer);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_blades);
        model->addMeleeWeapon(&m_spikes);
        addModel(model);
    }

    void PlaguePriestOnPlagueFurnace::onWounded() {
        Unit::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_spikes.setToHit(g_damageTable[damageIndex].m_spikesToHit);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void PlaguePriestOnPlagueFurnace::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t PlaguePriestOnPlagueFurnace::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds PlaguePriestOnPlagueFurnace::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                     int woundRoll) const {
        // Great Plague Censor
        if ((weapon->name() == m_censer.name()) && (hitRoll >= 2)) {
            return {0, Dice::RollD3() + g_damageTable[getDamageTableIndex()].m_censerDamage};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void PlaguePriestOnPlagueFurnace::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        // Poisonous Fumes
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto unit : units) {
            if (!unit->hasKeyword(CLANS_PESTILENS)) {
                int mortalWounds = 0;
                int roll = Dice::RollD6();
                if (roll == 6) mortalWounds = Dice::RollD3();
                else if (roll >= 4) mortalWounds = 1;

                unit->applyDamage({0, mortalWounds}, this);
            }
        }
    }

    int PlaguePriestOnPlagueFurnace::altarOfTheHornedRat(const Unit *unit) {
        // Altar of the Horned Rat
        if (unit->hasKeyword(SKAVENTIDE) && (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 13.0)) {
            // Make unit battleshock immune
            return 13;
        }

        return 0;
    }

    int PlaguePriestOnPlagueFurnace::extraAttacks(const Model *attackingModel, const Weapon *weapon,
                                                  const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // Pushed into Battle
        if ((weapon->name() == m_spikes.name()) && m_charged) {
            extra += Dice::RollD6();
        }
        return extra;
    }

    Wounds PlaguePriestOnPlagueFurnace::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Skaventide::applyWoundSave(wounds, attackingUnit);

        // Protection of the Horned Rat
        return ignoreWounds(totalWounds, 5);
    }

    void PlaguePriestOnPlagueFurnace::onStartHero(PlayerId player) {
        Skaventide::onStartHero(player);

        if (player == owningPlayer()) {
            // Noxious Prayers
            auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), CLANS_PESTILENS, 13.0);

            if (!unit) {
                // No other units - pray for this unit.
                unit = this;
            }

            auto prayerRoll = Dice::RollD6();
            if (prayerRoll == 1) {
                // Failed - take one mortal wound.
                applyDamage({0, 1}, this);
            } else if (prayerRoll >= 3) {
                // Success - select prayer (randomly)
                if (Dice::RollD6() >= 4) {
                    // Filth-filth!
                    unit->buffReroll(Attribute::To_Wound_Melee, Rerolls::Failed,
                                     {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                    unit->buffReroll(Attribute::To_Wound_Missile, Rerolls::Failed,
                                     {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                } else {
                    // Rabid-rabid!
                    unit->buffModifier(Attribute::Attacks_Melee, 1,
                                       {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                }
            }
        }

    }

    int PlaguePriestOnPlagueFurnace::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
