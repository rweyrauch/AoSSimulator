/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/MortisEngine.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Death {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 180;

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

    bool MortisEngine::s_registered = false;

    Unit *MortisEngine::Create(const ParameterList &parameters) {
        auto unit = new MortisEngine();

        auto legion = (Legion)GetEnumParam("Legion", parameters, GrandHostOfNagash);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int MortisEngine::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void MortisEngine::Init() {
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
                    {DEATHMAGES}
            };
            s_registered = UnitFactory::Register("Mortis Engine", factoryMethod);
        }
    }

    MortisEngine::MortisEngine() :
            LegionOfNagashBase("Mortis Engine", 14, WOUNDS, 10, 4, true),
            m_wail(Weapon::Type::Missile, "Wail of the Damned", 9, 1, 0, 0, 0, 0),
            m_staff(Weapon::Type::Melee, "Corpsemaster's Mortis Staff", 1, 1, 4, 3, -1, RAND_D3),
            m_etherealWeapons(Weapon::Type::Melee, "Spectral Host's Ethereal Weapons", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, DEATHMAGES, MORTIS_ENGINE};
        m_weapons = {&m_wail, &m_staff, &m_etherealWeapons};
    }

    bool MortisEngine::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_wail);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void MortisEngine::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_etherealWeapons.setAttacks(g_damageTable[damageIndex].m_hostAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void MortisEngine::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int MortisEngine::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds MortisEngine::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_etherealWeapons.name())) return {0, 1};
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void MortisEngine::onStartShooting(PlayerId player) {
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
