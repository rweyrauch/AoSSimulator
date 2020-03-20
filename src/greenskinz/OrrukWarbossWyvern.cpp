/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukWarbossWyvern.h>
#include <UnitFactory.h>

namespace Greenskinz {
    static const int BASESIZE = 100;
    static const int WOUNDS = 11;
    static const int POINTS_PER_UNIT = 240;

    struct TableEntry {
        int m_move;
        int m_clawsAttacks;
        int m_tailToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {12, 5, 2},
                    {10, 4, 3},
                    {8,  3, 4},
                    {6,  2, 5},
                    {4,  1, 6}
            };

    bool OrrukWarbossOnWyvern::s_registered = false;

    OrrukWarbossOnWyvern::OrrukWarbossOnWyvern() :
            Unit("Orruk Warboss on Wyvern", 12, WOUNDS, 7, 4, true),
            m_bossChoppa(Weapon::Type::Melee, "Boss Choppa", 1, 6, 3, 3, -1, 1),
            m_hornsClawsAndTeeth(Weapon::Type::Melee, "Wyvern's Horns, Claws and Teeth", 2, 5, 4, 3, -1, 2),
            m_barbedTail(Weapon::Type::Melee, "Wyvern's Barbed, Venomous Tail", 3, 2, 4, 2, -1, 3) {
        m_keywords = {DESTRUCTION, ORRUK, WYVERN, GREENSKINZ, MONSTER, HERO, ORRUK_WARBOSS};
        m_weapons = {&m_bossChoppa, &m_hornsClawsAndTeeth, &m_barbedTail};
    }

    bool OrrukWarbossOnWyvern::configure(bool pairedChoppas) {
        auto model = new Model(BASESIZE, wounds());

        m_pairedChoppas = pairedChoppas;

        model->addMeleeWeapon(&m_bossChoppa);
        model->addMeleeWeapon(&m_hornsClawsAndTeeth);
        model->addMeleeWeapon(&m_barbedTail);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *OrrukWarbossOnWyvern::Create(const ParameterList &parameters) {
        auto unit = new OrrukWarbossOnWyvern();
        bool pairedChoppas = GetBoolParam("Two Boss Choppas", parameters, false);

        bool ok = unit->configure(pairedChoppas);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void OrrukWarbossOnWyvern::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukWarbossOnWyvern::Create,
                    nullptr,
                    nullptr,
                    OrrukWarbossOnWyvern::ComputePoints,
                    {
                            {ParamType::Boolean, "Two Boss Choppas", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Orruk Warboss on Wyvern", factoryMethod);
        }
    }

    int OrrukWarbossOnWyvern::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void OrrukWarbossOnWyvern::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_hornsClawsAndTeeth.setAttacks(g_damageTable[damageIndex].m_clawsAttacks);
        m_barbedTail.setToWound(g_damageTable[damageIndex].m_tailToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    void OrrukWarbossOnWyvern::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Rerolls OrrukWarbossOnWyvern::toSaveRerolls(const Weapon *weapon) const {
        // Boss Shield
        if (!m_pairedChoppas) {
            return RerollFailed;
        }
        return Unit::toSaveRerolls(weapon);
    }

    int
    OrrukWarbossOnWyvern::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Unit::extraAttacks(nullptr, weapon, target);
        // Choppa Boss
        if (m_pairedChoppas) {
            attacks += 2;
        }
        return attacks;
    }

    int OrrukWarbossOnWyvern::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Greenskinz
