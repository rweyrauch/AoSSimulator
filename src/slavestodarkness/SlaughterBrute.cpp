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
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 170;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
        int m_jawsToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
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
        return POINTS_PER_UNIT;
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
            SlavesToDarknessBase("Slaughterbrute", 10, WOUNDS, 7, 4, false),
            m_claws(Weapon::Type::Melee, "Razor-tipped Claws", 2, 6, 4, 3, -1, RAND_D3),
            m_jaws(Weapon::Type::Melee, "Mighty Jaws", 1, 2, 4, 1, 0, 3),
            m_talons(Weapon::Type::Melee, "Slashing Talons", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MONSTER, SLAVES_TO_DARKNESS, SLAUGHTERBRUTE};
        m_weapons = {&m_claws, &m_jaws, &m_talons};
    }

    bool Slaughterbrute::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

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
        Unit::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    int Slaughterbrute::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

} // namespace SlavesToDarkness


