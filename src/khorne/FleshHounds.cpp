/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/FleshHounds.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool FleshHounds::s_registered = false;

    FleshHounds::FleshHounds(SlaughterHost host, int numModels) :
            KhorneBase("Flesh Hounds", 8, g_wounds, 10, 5, false) {
        m_keywords = {CHAOS, DAEMON, KHORNE, FLESH_HOUNDS};
        m_weapons = {&m_burningRoar, &m_blooddarkClaws};

        // Collars of Khorne
        m_totalUnbinds = 1;

        setSlaughterHost(host);

        int numGoreHounds = numModels / 5;
        // Add the Gore Hounds
        for (auto i = 0; i < numGoreHounds; i++) {
            auto goreHound = new Model(g_basesize, wounds());
            goreHound->addMissileWeapon(&m_burningRoar);
            goreHound->addMeleeWeapon(&m_blooddarkClaws);
            addModel(goreHound);
        }

        auto currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blooddarkClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Unit *FleshHounds::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);

        return new FleshHounds(host, numModels);
    }

    void FleshHounds::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FleshHounds::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    FleshHounds::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Flesh Hounds", factoryMethod);
        }
    }

    int FleshHounds::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int FleshHounds::unbindingModifier() const {
        auto mod = KhorneBase::unbindingModifier();
        // Collars of Khorne
        if (remainingModels() >= 10) mod++;
        return mod;
    }

} // namespace Khorne
