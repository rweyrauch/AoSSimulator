/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Gutrot.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 140;

    bool GutrotSpume::s_registered = false;

    Unit *GutrotSpume::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        auto general = GetBoolParam("General", parameters, false);
        return new GutrotSpume(legion, general);
    }

    void GutrotSpume::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GutrotSpume::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    GutrotSpume::ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Gutrot Spume", factoryMethod);
        }
    }

    GutrotSpume::GutrotSpume(PlagueLegion legion, bool isGeneral) :
            NurgleBase(legion,"Gutrot Spume", 4, g_wounds, 9, 3, false, g_pointsPerUnit),
            m_axe(Weapon::Type::Melee, "Rot-pocked Axe", 2, 4, 3, 2, -1, 2),
            m_tentacles(Weapon::Type::Melee, "Flailing Tentacles", 1, RAND_D3, 2, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, GUTROT_SPUME};
        m_weapons = {&m_axe, &m_tentacles};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axe);
        model->addMeleeWeapon(&m_tentacles);
        addModel(model);
    }

    int GutrotSpume::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
