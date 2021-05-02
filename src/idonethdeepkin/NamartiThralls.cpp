/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/NamartiThralls.h>
#include <UnitFactory.h>
#include <iostream>
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 320;

    bool NamartiThralls::s_registered = false;

    NamartiThralls::NamartiThralls(int points) :
            IdonethDeepkinBase("Namarti Thralls", 6, g_wounds, 6, 5, false, points),
            m_lanmariBlade(Weapon::Type::Melee, "Lanmari Blade", 1, 2, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, NAMARTI, THRALLS};
        m_weapons = {&m_lanmariBlade};
        m_battleFieldRole = Role::Battleline;
    }

    bool NamartiThralls::configure(int numModels, int numIconBearers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        if (numIconBearers > g_maxUnitSize / 10) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_lanmariBlade);
            if (numIconBearers >= 0) {
                model->setName(Model::IconBearer);
                numIconBearers--;
            }
            addModel(model);
        }

        return true;
    }

    Unit *NamartiThralls::Create(const ParameterList &parameters) {
        auto unit = new NamartiThralls(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numIconBearers = GetIntParam("Icon Bearers", parameters, 0);

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        bool ok = unit->configure(numModels, numIconBearers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void NamartiThralls::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Icon Bearers", 0, 0, g_maxUnitSize / 10, 1),
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Namarti Thralls", factoryMethod);
        }
    }

    int NamartiThralls::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace IdonethDeepkin