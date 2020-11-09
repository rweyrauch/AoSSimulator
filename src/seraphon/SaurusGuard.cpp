/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusGuard.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool SaurusGuard::s_registered = false;

    SaurusGuard::SaurusGuard() :
            SeraphonBase("Saurus Guard", 5, g_wounds, 8, 4, false),
            m_celestitePolearm(Weapon::Type::Melee, "Celestite Polearm", 1, 2, 3, 3, -1, 1),
            m_celestitePolearmAlpha(Weapon::Type::Melee, "Celestite Polearm", 1, 3, 3, 3, -1, 1),
            m_jaws(Weapon::Type::Melee, "Powerful Jaws", 1, 1, 5, 4, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SAURUS, SAURUS_GUARD};
        m_weapons = {&m_celestitePolearm, &m_celestitePolearmAlpha, &m_jaws};

        s_globalBraveryMod.connect(this, &SaurusGuard::stardrakeIcon, &m_connection);
    }

    SaurusGuard::~SaurusGuard() {
        m_connection.disconnect();
    }

    bool SaurusGuard::configure(int numModels, bool iconBearer, bool wardrum) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_wardrum = wardrum;

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
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SaurusGuard::Create(const ParameterList &parameters) {
        auto unit = new SaurusGuard();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
        bool wardrum = GetBoolParam("Wardrum", parameters, false);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure(numModels, iconBearer, wardrum);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            BoolParameter( "Wardrum"),
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
        if (m_wardrum) return Reroll_Failed;

        return SeraphonBase::chargeRerolls();
    }

    int SaurusGuard::stardrakeIcon(const Unit *target) {
        // Icon Bearer
        if (m_iconBearer && (target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 6.0)) {
            return -1;
        }

        return 0;
    }

} //namespace Seraphon
