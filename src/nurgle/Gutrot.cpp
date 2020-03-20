/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Gutrot.h"

namespace Nurgle {
    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    bool GutrotSpume::s_registered = false;

    Unit *GutrotSpume::Create(const ParameterList &parameters) {
        auto unit = new GutrotSpume();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GutrotSpume::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GutrotSpume::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    GutrotSpume::ComputePoints,
                    {
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Gutrot Spume", factoryMethod);
        }
    }

    GutrotSpume::GutrotSpume() :
            NurgleBase("Gutrot Spume", 4, WOUNDS, 9, 3, false),
            m_axe(Weapon::Type::Melee, "Rot-pocked Axe", 2, 4, 3, 2, -1, 2),
            m_tentacles(Weapon::Type::Melee, "Flailing Tentacles", 1, RAND_D3, 2, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, GUTROT_SPUME};
        m_weapons = {&m_axe, &m_tentacles};
    }

    bool GutrotSpume::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_axe);
        model->addMeleeWeapon(&m_tentacles);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int GutrotSpume::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
