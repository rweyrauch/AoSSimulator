/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/SpiritOfTheMountain.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int BASESIZE = 105; // oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 340;

    struct TableEntry {
        int m_blastRange;
        int m_hammerDamage;
        double m_shockwave;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 10, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {30, 5, 12},
                    {25,  4,  10},
                    {20,  3,  8},
                    {15,  2,  6},
                    {10,  1,  4}
            };

    bool AlarithSpiritOfTheMountain::s_registered = false;

    AlarithSpiritOfTheMountain::AlarithSpiritOfTheMountain() :
            LuminethBase("Alarith Spirit of the Mountain", 6, WOUNDS, 10, 3, false),
            m_blast(Weapon::Type::Missile, "Geomantic Blast", 30, 1, 3, 2, -2, RAND_D6),
            m_hammer(Weapon::Type::Melee, "Stoneheart Worldhammer", 3, 4, 3, 2, -2, 5),
            m_hooves(Weapon::Type::Melee, "Cloven Hooves", 1, 2, 3, 3, -1, 2) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, HERO, ALARITH, MONSTER, SPIRIT_OF_THE_MOUNTAIN};
        m_weapons = {&m_blast, &m_hammer, &m_hooves};
        m_battleFieldRole = Role::LeaderBehemoth;
    }

    bool AlarithSpiritOfTheMountain::configure() {

        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_blast);
        model->addMeleeWeapon(&m_hammer);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *AlarithSpiritOfTheMountain::Create(const ParameterList &parameters) {
        auto unit = new AlarithSpiritOfTheMountain();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AlarithSpiritOfTheMountain::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Alarith Spirit of the Mountain", factoryMethod);
        }
    }

    int AlarithSpiritOfTheMountain::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void AlarithSpiritOfTheMountain::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void AlarithSpiritOfTheMountain::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_blast.setRange(g_damageTable[damageIndex].m_blastRange);
        m_hammer.setDamage(g_damageTable[damageIndex].m_hammerDamage);

        LuminethBase::onWounded();
    }

    int AlarithSpiritOfTheMountain::getDamageTableIndex() const {

        // Stonemage Symbiosis
        auto mage = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), STONEMAGE, 12.0f);
        if (mage) return 0;

        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int AlarithSpiritOfTheMountain::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // all but Immovable
        if (!m_charged && !weapon->isMissile()) extra++;

        return extra;
    }

    void AlarithSpiritOfTheMountain::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        // Stoneheart Shockwave
        if (player != owningPlayer()) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), g_damageTable[getDamageTableIndex()].m_shockwave);
            for (auto ip : units) {
                const Weapon* missileWeapon = nullptr;
                if (ip->hasShootingAttack(&missileWeapon)) {
                    ip->buffModifier(ToHitMissile, -1, {Shooting, m_battleRound, player});
                    break;
                }
            }
        }
    }

    void AlarithSpiritOfTheMountain::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Stoneheart Shockwave
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), g_damageTable[getDamageTableIndex()].m_shockwave);
        for (auto ip : units) {
            ip->buffModifier(ToHitMelee, -1, {Combat, m_battleRound, player});
            break;
        }
    }

} // namespace LuminethRealmLords