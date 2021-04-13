/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/CorvusCabal.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 9;
    static const int g_maxUnitSize = 36;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 280;

    bool CorvusCabal::s_registered = false;

    Unit *CorvusCabal::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new CorvusCabal(legion, numModels);
    }

    void CorvusCabal::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CorvusCabal::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    CorvusCabal::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Corvus Cabal", factoryMethod);
        }
    }

    CorvusCabal::CorvusCabal(DamnedLegion legion, int numModels) :
            SlavesToDarknessBase("Corvus Cabal", 8, g_wounds, 5, 6, true) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, CORVUS_CABAL};
        m_weapons = {&m_ravenDarts, &m_corvusWeapons, &m_corvusWeaponsLeader};

        setDamnedLegion(legion);

        auto piercer = new Model(g_basesize, wounds());
        piercer->addMissileWeapon(&m_ravenDarts);
        piercer->addMeleeWeapon(&m_corvusWeaponsLeader);
        piercer->setName("Shadow Piercer");
        addModel(piercer);

        auto talon = new Model(g_basesize, wounds());
        talon->addMissileWeapon(&m_ravenDarts);
        talon->addMeleeWeapon(&m_corvusWeapons);
        talon->setName("Shrike Talon");
        addModel(talon);

        for (auto i = 2; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_ravenDarts);
            model->addMeleeWeapon(&m_corvusWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Rerolls CorvusCabal::chargeRerolls() const {
        if (isNamedModelAlive("Shrike Talon")) {
            return Rerolls::Ones;
        }
        return SlavesToDarknessBase::chargeRerolls();
    }

    int CorvusCabal::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //SlavesToDarkness