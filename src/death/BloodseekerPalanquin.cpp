/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/BloodseekerPalangquin.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Death {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 300;

    struct TableEntry {
        int m_move;
        float m_wailRange;
        int m_hostAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 9, 12},
                    {12, 8, 10},
                    {10, 7, 8},
                    {8, 6, 6},
                    {4, 5, 4}
            };

    bool BloodseekerPalanquin::s_registered = false;

    Unit *BloodseekerPalanquin::Create(const ParameterList &parameters) {
        auto unit = new BloodseekerPalanquin();

        auto legion = (Legion)GetEnumParam("Legion", parameters, GrandHostOfNagash);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int BloodseekerPalanquin::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void BloodseekerPalanquin::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Legion", Legion::GrandHostOfNagash, Legion ::GrandHostOfNagash, Legion::LegionOfBlood, 1},
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Bloodseeker Palanquin", factoryMethod);
        }
    }

    BloodseekerPalanquin::BloodseekerPalanquin() :
            LegionOfNagashBase("Bloodseeker Palanquin", 14, WOUNDS, 10, 4, true),
            m_wail(Weapon::Type::Missile, "Wail of the Damned", 9, 1, 0, 0, -7, 0),
            m_blade(Weapon::Type::Melee, "Sanguinarch's Bloodletting Blade", 1, 4, 3, 3, -1, RAND_D3),
            m_etherealWeapons(Weapon::Type::Melee, "Spectral Host's Ethereal Weapons", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, MALIGNANT, HERO, WIZARD, BLOODSEEKER_PALANQUIN};
        m_weapons = {&m_wail, &m_blade, &m_etherealWeapons};
    }

    bool BloodseekerPalanquin::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_wail);
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void BloodseekerPalanquin::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_etherealWeapons.setAttacks(g_damageTable[damageIndex].m_hostAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void BloodseekerPalanquin::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int BloodseekerPalanquin::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds BloodseekerPalanquin::weaponDamage(const Weapon *weapon, const Unit *target,
            int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_etherealWeapons.name())) return {0, 1};
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void BloodseekerPalanquin::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0f);
    }

    void BloodseekerPalanquin::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        // Wail of the Damned
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), g_damageTable[getDamageTableIndex()].m_wailRange);
        for (auto unit : units) {
            if (Dice::roll2D6() > unit->bravery()) {
                unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }

} // namespace Death
