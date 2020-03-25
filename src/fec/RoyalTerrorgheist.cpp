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

namespace FleshEaterCourt {
    static const int BASESIZE = 130;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    struct TableEntry {
        int m_move;
        int m_deathShriek;
        int m_clawAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 7, 10, 13, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 6, 4},
                    {12, 5, 4},
                    {10, 4, 3},
                    {8,  3, 3},
                    {6,  2, 2}
            };

    bool RoyalTerrorgheist::s_registered = false;

    RoyalTerrorgheist::RoyalTerrorgheist() :
            FleshEaterCourts("Royal Terrorgheist", 14, WOUNDS, 10, 4, true),
            m_deathShriek(Weapon::Type::Missile, "Death Shriek", 10, 1, 0, 0, 0, 0),
            m_skeletalClaws(Weapon::Type::Melee, "Skeletal Claws", 2, 4, 4, 3, -1, RAND_D3),
            m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 3, 3, 4, 3, -2, RAND_D6) {
        m_keywords = {DEATH, FLESH_EATER_COURTS, MENAGERIE, MONSTER, ROYAL_TERRORGHEIST};
        m_weapons = {&m_deathShriek, &m_skeletalClaws, &m_fangedMaw};
    }

    bool RoyalTerrorgheist::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_deathShriek);
        model->addMeleeWeapon(&m_skeletalClaws);
        model->addMeleeWeapon(&m_fangedMaw);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *RoyalTerrorgheist::Create(const ParameterList &parameters) {
        auto unit = new RoyalTerrorgheist();

        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, NoCourt);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, None);
        // TODO: error checks - can only select delusion if GrandCourt is NoCourt.
        unit->setGrandCourt(court);
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
                            {ParamType::Enum, "Grand Court", FleshEaterCourts::NoCourt, FleshEaterCourts::NoCourt,
                             FleshEaterCourts::Gristlegore, 1},
                            {ParamType::Enum, "Delusion", FleshEaterCourts::None, FleshEaterCourts::None,
                             FleshEaterCourts::DefendersOfTheRealm, 1},
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
            auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), ABHORRANT, 6.0f);
            if (unit != nullptr) {
                if (remainingWounds() < WOUNDS && remainingWounds() > 0) {
                    int woundsHealed = Dice::rollD3();
                    for (auto &m : m_models) {
                        m->applyHealing(woundsHealed);
                    }
                }
            }
        }
    }

    int RoyalTerrorgheist::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
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
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0f);
        for (auto ip : units) {
            Wounds wounds = {0, Dice::rollD3()};
            ip->applyDamage(wounds);
        }
    }

    int RoyalTerrorgheist::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

    void RoyalTerrorgheist::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        // Death Shriek
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= m_deathShriek.range())) {
            const auto roll = Dice::rollD6() + g_damageTable[getDamageTableIndex()].m_deathShriek;
            if (roll > unit->bravery()) {
                unit->applyDamage({0, roll-unit->bravery()});
            }
        }
    }

} // namespace FleshEasterCourt
