/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Slambo.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool Slambo::s_registered = false;

    Unit *Slambo::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Slambo(legion, general);
    }

    void Slambo::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Slambo::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Slambo::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Slambo", factoryMethod);
        }
    }

    Slambo::Slambo(DamnedLegion legion, bool isGeneral) :
            SlavesToDarknessBase("Slambo", 5, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, EXALTED_HERO_OF_CHAOS, SLAMBO};
        m_weapons = {&m_hurledAxe, &m_chaosAxes};
        m_battleFieldRole = Role::Leader;

        setDamnedLegion(legion);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_hurledAxe);
        model->addMeleeWeapon(&m_chaosAxes);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    int Slambo::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //SlavesToDarkness