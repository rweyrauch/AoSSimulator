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
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 180;

    struct TableEntry {
        int m_move;
        double m_wailRange;
        int m_hostAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 9, 12},
                    {12, 8, 10},
                    {10, 7, 8},
                    {8,  6, 6},
                    {4,  5, 4}
            };

    bool MortisEngine::s_registered = false;

    Unit *MortisEngine::Create(const ParameterList &parameters) {
        auto unit = new MortisEngine();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int MortisEngine::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MortisEngine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {DEATHMAGES}
            };
            s_registered = UnitFactory::Register("Mortis Engine", factoryMethod);
        }
    }

    MortisEngine::MortisEngine() :
            LegionOfNagashBase("Mortis Engine", 14, g_wounds, 10, 4, true),
            m_wail(Weapon::Type::Missile, "Wail of the Damned", 9, 1, 0, 0, 0, 0),
            m_staff(Weapon::Type::Melee, "Corpsemaster's Mortis Staff", 1, 1, 4, 3, -1, RAND_D3),
            m_etherealWeapons(Weapon::Type::Melee, "Spectral Host's Ethereal Weapons", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, DEATHMAGES, MORTIS_ENGINE};
        m_weapons = {&m_wail, &m_staff, &m_etherealWeapons};
        m_battleFieldRole = Role::Behemoth;
    }

    bool MortisEngine::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_wail);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void MortisEngine::onWounded() {
        LegionOfNagashBase::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_etherealWeapons.setAttacks(g_damageTable[damageIndex].m_hostAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void MortisEngine::onRestore() {
        LegionOfNagashBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int MortisEngine::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds MortisEngine::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_etherealWeapons.name())) return {0, 1};
        return LegionOfNagashBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void MortisEngine::onStartShooting(PlayerId player) {
        LegionOfNagashBase::onStartShooting(player);

        // Wail of the Damned
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()),
                                                       g_damageTable[getDamageTableIndex()].m_wailRange);
        for (auto unit : units) {
            if (Dice::Roll2D6() > unit->bravery()) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

} // namespace Death
