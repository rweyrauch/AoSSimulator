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
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 80;

    bool WarpGrinder::s_registered = false;

    Unit *WarpGrinder::Create(const ParameterList &parameters) {
        auto unit = new WarpGrinder();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
            Skaventide("Warp-grinder", 6, WOUNDS, 4, 6, false),
            m_warpGrinder(Weapon::Type::Melee, "Warp-grinder", 1, 1, 4, 3, -2, 2) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, WARP_GRINDER};
        m_weapons = {&m_warpGrinder};
    }

    bool WarpGrinder::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_warpGrinder);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int WarpGrinder::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }
} //namespace Skaven
