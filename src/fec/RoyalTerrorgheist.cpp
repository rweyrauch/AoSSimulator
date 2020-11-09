/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/RoyalTerrorgheist.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 130;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 300;

    struct TableEntry {
        int m_move;
        int m_deathShriek;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {4, 7, 10, 13, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 6, 4},
                    {12, 5, 4},
                    {10, 4, 3},
                    {8,  3, 3},
                    {6,  2, 2}
            };

    bool RoyalTerrorgheist::s_registered = false;

    RoyalTerrorgheist::RoyalTerrorgheist() :
            FleshEaterCourts("Royal Terrorgheist", 14, g_wounds, 10, 4, true),
            m_deathShriek(Weapon::Type::Missile, "Death Shriek", 10, 1, 0, 0, 0, 0),
            m_skeletalClaws(Weapon::Type::Melee, "Skeletal Claws", 2, 4, 4, 3, -1, RAND_D3),
            m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 3, 3, 4, 3, -2, RAND_D6) {
        m_keywords = {DEATH, FLESH_EATER_COURTS, MENAGERIE, MONSTER, ROYAL_TERRORGHEIST};
        m_weapons = {&m_deathShriek, &m_skeletalClaws, &m_fangedMaw};
        m_battleFieldRole = Behemoth;
    }

    bool RoyalTerrorgheist::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_deathShriek);
        model->addMeleeWeapon(&m_skeletalClaws);
        model->addMeleeWeapon(&m_fangedMaw);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *RoyalTerrorgheist::Create(const ParameterList &parameters) {
        auto unit = new RoyalTerrorgheist();

        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);

        // Can only select delusion if GrandCourt is NoCourt.
        unit->setGrandCourt(court);
        if (court == GrandCourt::None)
            unit->setCourtsOfDelusion(delusion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void RoyalTerrorgheist::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    RoyalTerrorgheist::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    RoyalTerrorgheist::ComputePoints,
                    {
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Royal Terrorgheist", factoryMethod);
        }
    }

    Wounds RoyalTerrorgheist::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Gaping Maw
        if (weapon->name() == m_fangedMaw.name() && hitRoll == 6) {
            return {0, 6};
        }
        return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void RoyalTerrorgheist::onStartHero(PlayerId player) {
        // Royal Menagerie
        if (player == owningPlayer()) {
            auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), ABHORRANT, 6.0);
            if (unit != nullptr) {
                if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                    int woundsHealed = Dice::RollD3();
                    for (auto &m : m_models) {
                        m->applyHealing(woundsHealed);
                    }
                }
            }
        }
    }

    int RoyalTerrorgheist::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void RoyalTerrorgheist::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_skeletalClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void RoyalTerrorgheist::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    void RoyalTerrorgheist::onSlain() {
        FleshEaterCourts::onSlain();

        // Infested
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        for (auto ip : units) {
            Wounds wounds = {0, Dice::RollD3()};
            ip->applyDamage(wounds);
        }
    }

    int RoyalTerrorgheist::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void RoyalTerrorgheist::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        // Death Shriek
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= (double) m_deathShriek.range())) {
            const auto roll = Dice::RollD6() + g_damageTable[getDamageTableIndex()].m_deathShriek;
            if (roll > unit->bravery()) {
                unit->applyDamage({0, roll - unit->bravery()});
            }
        }
    }

} // namespace FleshEasterCourt
