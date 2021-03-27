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
        auto unit = new Mourngul();

        unit->configure();
        return unit;
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
            Nighthaunt("Mourngul", 12, g_wounds, 10, 4, true),
            m_clawsAndFangs(Weapon::Type::Melee, "Nightmarish Claws and Fangs", 2, 8, 3, 3, -1, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, MONSTER, MOURNGUL};
        m_weapons = {&m_clawsAndFangs};
    }

    void Mourngul::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);

        m_points = ComputePoints(1);
    }

    void Mourngul::onWounded() {
        Unit::onWounded();

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

    Wounds Mourngul::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightfull Touch
        if (hitRoll == 6) {
            return {0, 2};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Mourngul::onRestore() {
        Unit::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    int Mourngul::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
