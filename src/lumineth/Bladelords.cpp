/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Bladelords.h>
#include <UnitFactory.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool VanariBladelords::s_registered = false;

    Unit *VanariBladelords::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto dualBlades = GetBoolParam("Seneschal Dual Blades", parameters, false);
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        return new VanariBladelords(numModels, dualBlades, nation);
    }

    int VanariBladelords::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void VanariBladelords::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Seneschal Dual Blades"),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Bladelords", factoryMethod);
        }
    }

    VanariBladelords::VanariBladelords(int numModels, bool seneschalDualBlades, GreatNation nation) :
            LuminethBase("Vanari Bladelords", 6, g_wounds, 7, 4, false) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, BLADELORDS};
        m_weapons = {&m_greatbladeStrike, &m_greatbladeSeneschalStrike, &m_greatbladeBlows, &m_greatbladeSeneschalBlow, &m_dualBlades};

        setNation(nation);

        auto seneschal = new Model(g_basesize, wounds());
        if (seneschalDualBlades) {
            seneschal->addMeleeWeapon(&m_dualBlades);
        }
        else {
            seneschal->addMeleeWeapon(&m_greatbladeSeneschalStrike);
            seneschal->addMeleeWeapon(&m_greatbladeSeneschalBlow);
        }
        seneschal->setName("Bladelord Seneschal");
        addModel(seneschal);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_greatbladeStrike);
            model->addMeleeWeapon(&m_greatbladeBlows);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }
}