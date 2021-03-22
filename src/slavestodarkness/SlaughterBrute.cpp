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
        auto unit = new Slaughterbrute();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    Slaughterbrute::Slaughterbrute() :
            SlavesToDarknessBase("Slaughterbrute", 10, g_wounds, 7, 4, false),
            m_claws(Weapon::Type::Melee, "Razor-tipped Claws", 2, 6, 4, 3, -1, RAND_D3),
            m_jaws(Weapon::Type::Melee, "Mighty Jaws", 1, 2, 4, 1, 0, 3),
            m_talons(Weapon::Type::Melee, "Slashing Talons", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MONSTER, SLAVES_TO_DARKNESS, SLAUGHTERBRUTE};
        m_weapons = {&m_claws, &m_jaws, &m_talons};
        m_battleFieldRole = Role::Behemoth;
    }

    bool Slaughterbrute::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void Slaughterbrute::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void Slaughterbrute::onRestore() {
        SlavesToDarknessBase::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    int Slaughterbrute::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

} // namespace SlavesToDarkness


