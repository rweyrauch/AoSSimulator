/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Slaughterbrute.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 170;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
        int m_jawsToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 6, 1},
                    {8,  5, 2},
                    {8,  4, 3},
                    {6,  3, 4},
                    {4,  2, 5}
            };

    bool Slaughterbrute::s_registered = false;

    Unit *Slaughterbrute::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        return new Slaughterbrute(legion, mark);
    }

    int Slaughterbrute::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void Slaughterbrute::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Slaughterbrute::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Slaughterbrute::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE}
            };
            s_registered = UnitFactory::Register("Slaughterbrute", factoryMethod);
        }
    }

    Slaughterbrute::Slaughterbrute(DamnedLegion legion, MarkOfChaos mark) :
            SlavesToDarknessBase("Slaughterbrute", 10, g_wounds, 7, 4, false) {
        m_keywords = {CHAOS, MONSTER, SLAVES_TO_DARKNESS, SLAUGHTERBRUTE};
        m_weapons = {&m_claws, &m_jaws, &m_talons};
        m_battleFieldRole = Role::Behemoth;

        setDamnedLegion(legion);
        setMarkOfChaos(mark);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    void Slaughterbrute::onWounded() {
        Unit::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void Slaughterbrute::onRestore() {
        SlavesToDarknessBase::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    size_t Slaughterbrute::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

} // namespace SlavesToDarkness


