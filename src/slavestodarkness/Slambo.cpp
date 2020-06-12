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
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    bool Slambo::s_registered = false;

    Unit *Slambo::Create(const ParameterList &parameters) {
        auto unit = new Slambo();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Slambo", factoryMethod);
        }
    }

    Slambo::Slambo() :
            SlavesToDarknessBase("Slambo", 5, WOUNDS, 8, 4, false),
            m_hurledAxe(Weapon::Type::Missile, "Hurled Chaos Axe", 8, 1, 3, 3, -1, RAND_D3),
            m_chaosAxes(Weapon::Type::Melee, "Chaos Axes", 1, RAND_D6, 4, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, EXALTED_HERO_OF_CHAOS, SLAMBO};
        m_weapons = {&m_hurledAxe, &m_chaosAxes};
    }

    bool Slambo::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_hurledAxe);
        model->addMeleeWeapon(&m_chaosAxes);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int Slambo::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //SlavesToDarkness