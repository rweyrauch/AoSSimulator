/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <soulblight/Terrorgheist.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
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

    bool Terrorgheist::s_registered = false;

    Terrorgheist::Terrorgheist(CursedBloodline bloodline, Mutation mutation) :
            SoulblightBase(bloodline, "Terrorgheist", 14, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_deathShriek(Weapon::Type::Missile, "Death Shriek", 10, 1, 0, 0, 0, 0),
            m_skeletalClaws(Weapon::Type::Melee, "Skeletal Claws", 2, 4, 4, 3, -1, RAND_D3),
            m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 3, 3, 4, 3, -2, RAND_D6) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, MONSTER, TERRORGHEIST};
        m_weapons = {&m_deathShriek, &m_skeletalClaws, &m_fangedMaw};
        m_battleFieldRole = (bloodline == CursedBloodline::Avengorii_Dynasty) ? Role::Battleline_Behemoth : Role::Behemoth;

        setMutation(mutation);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_deathShriek);
        model->addMeleeWeapon(&m_skeletalClaws);
        model->addMeleeWeapon(&m_fangedMaw);
        addModel(model);
    }

    Unit *Terrorgheist::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto mutation = (Mutation) GetEnumParam("Cursed Mutation", parameters, g_mutations[0]);
        return new Terrorgheist(bloodline, mutation);
    }

    void Terrorgheist::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Terrorgheist::Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    Terrorgheist::ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                            EnumParameter("Cursed Mutation", g_mutations[0], g_mutations)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Terrorgheist", factoryMethod);
        }
    }

    Wounds Terrorgheist::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Gaping Maw
        if (weapon->name() == m_fangedMaw.name() && hitRoll == 6) {
            return {0, 6};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void Terrorgheist::onStartHero(PlayerId player) {
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

    size_t Terrorgheist::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Terrorgheist::onWounded() {
        SoulblightBase::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_skeletalClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void Terrorgheist::onRestore() {
        SoulblightBase::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    void Terrorgheist::onFriendlyUnitSlain(const Unit *attacker) {
        SoulblightBase::onFriendlyUnitSlain(nullptr);

        // Infested
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        for (auto ip : units) {
            Wounds wounds = {0, Dice::RollD3()};
            ip->applyDamage(wounds, this);
        }
    }

    int Terrorgheist::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Terrorgheist::onStartShooting(PlayerId player) {
        SoulblightBase::onStartShooting(player);

        // Death Shriek
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= (double) m_deathShriek.range())) {
            const auto roll = Dice::RollD6() + g_damageTable[getDamageTableIndex()].m_deathShriek;
            if (roll > unit->bravery()) {
                unit->applyDamage({0, roll - unit->bravery()}, this);
            }
        }
    }

} // namespace Soulblight
