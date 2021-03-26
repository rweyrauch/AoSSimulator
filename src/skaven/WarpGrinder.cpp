/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarpGrinder.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 3;
    static const int g_pointsPerUnit = 80;

    bool WarpGrinder::s_registered = false;

    Unit *WarpGrinder::Create(const ParameterList &parameters) {
        auto unit = new WarpGrinder();

        unit->configure();
        return unit;
    }

    void WarpGrinder::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Warp-grinder", factoryMethod);
        }
    }

    WarpGrinder::WarpGrinder() :
            Skaventide("Warp-grinder", 6, g_wounds, 4, 6, false),
            m_warpGrinder(Weapon::Type::Melee, "Warp-grinder", 1, 1, 4, 3, -2, 2) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, WARP_GRINDER};
        m_weapons = {&m_warpGrinder};
    }

    bool WarpGrinder::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_warpGrinder);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int WarpGrinder::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }
} //namespace Skaven
