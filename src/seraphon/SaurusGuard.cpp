/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusGuard.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool SaurusGuard::s_registered = false;

    SaurusGuard::SaurusGuard(WayOfTheSeraphon way, Constellation constellation, int numModels, bool iconBearer, bool wardrum) :
            SeraphonBase("Saurus Guard", 5, g_wounds, 8, 4, false) {
        m_keywords = {ORDER, SERAPHON, SAURUS, SAURUS_GUARD};
        m_weapons = {&m_celestitePolearm, &m_celestitePolearmAlpha, &m_jaws};

        setWayOfTheSeraphon(way, constellation);

        s_globalBraveryMod.connect(this, &SaurusGuard::stardrakeIcon, &m_connection);

        // Add the Alpha
        auto alpha = new Model(g_basesize, wounds());
        alpha->addMeleeWeapon(&m_celestitePolearmAlpha);
        alpha->addMeleeWeapon(&m_jaws);
        addModel(alpha);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_celestitePolearm);
            model->addMeleeWeapon(&m_jaws);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (wardrum) {
                model->setName("Wardrum");
                wardrum = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    SaurusGuard::~SaurusGuard() {
        m_connection.disconnect();
    }

    Unit *SaurusGuard::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
        bool wardrum = GetBoolParam("Wardrum", parameters, false);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);

        return new SaurusGuard(way, constellation, numModels, iconBearer, wardrum);
    }

    void SaurusGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Stardrake Icon"),
                            BoolParameter("Wardrum"),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Guard", factoryMethod);
        }
    }

    int SaurusGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls SaurusGuard::chargeRerolls() const {
        if (isNamedModelAlive("Wardrum")) return Rerolls::Failed;

        return SeraphonBase::chargeRerolls();
    }

    int SaurusGuard::stardrakeIcon(const Unit *target) {
        // Icon Bearer
        if (isNamedModelAlive(Model::IconBearer) && (target->owningPlayer() != owningPlayer()) &&
            (distanceTo(target) <= 6.0)) {
            return -1;
        }

        return 0;
    }

} //namespace Seraphon
