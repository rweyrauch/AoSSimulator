/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosWarshrine.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 170;

    struct TableEntry {
        int m_move;
        int m_fistAttacks;
        int m_protection;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {8, 6, 18},
                    {7, 5, 12},
                    {6, 4, 9},
                    {5, 3, 6},
                    {4, 2, 3}
            };

    bool ChaosWarshrine::s_registered = false;

    Unit *ChaosWarshrine::Create(const ParameterList &parameters) {
        auto unit = new ChaosWarshrine();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, Undivided);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosWarshrine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosWarshrine::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosWarshrine::ComputePoints,
                    {
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                            {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided,
                             SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Warshrine", factoryMethod);
        }
    }

    ChaosWarshrine::ChaosWarshrine() :
            SlavesToDarknessBase("Chaos Warshrine", 8, WOUNDS, 7, 4, false),
            m_blade(Weapon::Type::Melee, "Sacrificial Blade", 1, 4, 3, 3, -1, 2),
            m_fists(Weapon::Type::Melee, "Flailing Fists", 1, 6, 4, 3, 0, 2) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, TOTEM, PRIEST, MARK_OF_CHAOS, CHAOS_WARSHRINE};
        m_weapons = {&m_blade, &m_fists};
    }

    bool ChaosWarshrine::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_fists);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void ChaosWarshrine::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    void ChaosWarshrine::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_fists.setAttacks(g_damageTable[damageIndex].m_fistAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int ChaosWarshrine::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int ChaosWarshrine::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace SlavesToDarkness
