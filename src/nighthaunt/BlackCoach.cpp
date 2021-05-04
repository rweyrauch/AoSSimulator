/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "nighthaunt/BlackCoach.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 170; // x105 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 220;

    bool BlackCoach::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 9},
                    {10, 8},
                    {8,  7},
                    {6,  6},
                    {4,  5}
            };

    Unit *BlackCoach::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        return new BlackCoach(procession);
    }

    void BlackCoach::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BlackCoach::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    BlackCoach::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Black Coach", factoryMethod);
        }
    }

    BlackCoach::BlackCoach(Procession procession) :
            Nighthaunt(procession, "Black Coach", 14, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_graspMissile(Weapon::Type::Missile, "Cairn Wraith's Soulreach Grasp", 10, 1, 3, 3, -3, RAND_D3),
            m_scythe(Weapon::Type::Melee, "Cairn Wraith's Reaper Scythe", 1, 3, 4, 3, -1, 2),
            m_grasp(Weapon::Type::Melee, "Cairn Wraith's Soulreach Grasp", 3, 1, 3, 3, -3, RAND_D3),
            m_claws(Weapon::Type::Melee, "Relic Bearers' Spectral Claws", 1, 9, 4, 4, -1, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Nightmares' Hooves and Teeth", 1, 8, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, BLACK_COACH};
        m_weapons = {&m_graspMissile, &m_scythe, &m_grasp, &m_claws, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Behemoth;
        m_hasMount = true;
        m_claws.setMount(true);
        m_hoovesAndTeeth.setMount(true);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_graspMissile);
        model->addMeleeWeapon(&m_scythe);
        model->addMeleeWeapon(&m_grasp);
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);
    }

    void BlackCoach::onWounded() {
        Nighthaunt::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[damageIndex].m_move;
    }

    size_t BlackCoach::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds BlackCoach::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightfull Touch
        if (hitRoll == 6) {
            if (weapon->name() == m_scythe.name()) {
                return {0, 2};
            } else if (weapon->name() == m_claws.name()) {
                return {0, 1};
            }
        }
        return Nighthaunt::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void BlackCoach::onBeginRound(int battleRound) {
        Nighthaunt::onBeginRound(battleRound);

        // Evocation of Death
        Dice::RollResult result;
        Dice::RollD6(3, result);
        m_powerLevel += result.rollsGE(4);

        // Insubstantial Form
        if (m_powerLevel >= 4) {
            m_retreatAndCharge = true;
        }

        // Unholy Vigour
        if (m_powerLevel >= 2) {
            m_runAndCharge = true;
        }
    }

    void BlackCoach::onStartHero(PlayerId player) {
        Nighthaunt::onStartHero(player);

        if ((player == owningPlayer()) && (m_powerLevel >= 1)) {
            // Nimbus of Power
            heal(Dice::RollD3());
        }

        if ((player == owningPlayer()) && (m_powerLevel >= 5)) {
            // Witch-fire
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 4) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }
    }

    void BlackCoach::onCharged() {
        Nighthaunt::onCharged();

        if (m_charged && (m_powerLevel >= 3)) {
            // Spectral Scythes
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            if (!units.empty()) {
                if (Dice::RollD6() >= 2) {
                    units[0]->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }
    }

    void BlackCoach::onRestore() {
        Nighthaunt::onRestore();

        m_powerLevel = 0;
        m_runAndCharge = false;
        m_retreatAndCharge = false;

        // Reset table-drive attributes
        onWounded();
    }

    Rerolls BlackCoach::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Reaped Like Corn
        if ((target->remainingModels() >= 5) && (weapon->name() == m_scythe.name())) {
            return Rerolls::Failed;
        }

        // Unholy Vigour
        if ((m_powerLevel >= 2) && (!weapon->isMissile())) {
            return Rerolls::Ones;
        }
        return Nighthaunt::toHitRerolls(weapon, target);
    }

    int BlackCoach::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
