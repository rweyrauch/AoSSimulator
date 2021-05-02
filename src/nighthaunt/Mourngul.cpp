/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "nighthaunt/Mourngul.h"

namespace Nighthaunt {
    static const int g_basesize = 170; // x105 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 270;

    bool Mourngul::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 8},
                    {10, 7},
                    {9,  6},
                    {8,  5},
                    {7,  4}
            };

    Unit *Mourngul::Create(const ParameterList &parameters) {
        return new Mourngul();
    }

    void Mourngul::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Mourngul::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    Mourngul::ComputePoints,
                    {
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Black Coach", factoryMethod);
        }
    }

    Mourngul::Mourngul() :
            Nighthaunt("Mourngul", 12, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, MONSTER, MOURNGUL};
        m_weapons = {&m_clawsAndFangs};

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);
    }

    void Mourngul::onWounded() {
        Nighthaunt::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_clawsAndFangs.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[damageIndex].m_move;
    }

    size_t Mourngul::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds Mourngul::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if (hitRoll == 6) {
            return {0, 2};
        }
        return Nighthaunt::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void Mourngul::onRestore() {
        Nighthaunt::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    int Mourngul::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Mourngul::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon, const Wounds &weaponDamage) {
        Nighthaunt::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        // Devourer of Flesh and Souls
        if ((numSlain > 0) && (weapon->name() == m_clawsAndFangs.name())) {
            heal(Dice::RollD3());
        }
    }

    int Mourngul::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Nighthaunt::targetHitModifier(weapon, attacker);
        if (distanceTo(attacker) < 6.0) {
            mod--;
        }
        return mod;
    }

} // namespace Nighthaunt
