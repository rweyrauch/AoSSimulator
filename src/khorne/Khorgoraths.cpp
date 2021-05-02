/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/Khorgoraths.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 8;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 600;

    bool Khorgoraths::s_registered = false;

    Khorgoraths::Khorgoraths(SlaughterHost host, int numModels, int points) :
            KhorneBase("Khorgoraths", 6, g_wounds, 6, 4, false, points) {
        m_keywords = {CHAOS, KHORNE, MONSTER, BLOODBOUND, KHORGORATHS};
        m_weapons = {&m_boneTentacles, &m_clawAndFangs};

        setSlaughterHost(host);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_boneTentacles);
            model->addMeleeWeapon(&m_clawAndFangs);
            addModel(model);
        }
    }

    Unit *Khorgoraths::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);

        return new Khorgoraths(host, numModels, ComputePoints(parameters));
    }

    void Khorgoraths::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Khorgoraths::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Khorgoraths::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Khorgoraths", factoryMethod);
        }
    }

    void Khorgoraths::onStartCombat(PlayerId player) {
        m_modelsSlainAtStartOfCombat = m_currentRecord.m_enemyModelsSlain;

        KhorneBase::onStartCombat(player);
    }

    void Khorgoraths::onEndCombat(PlayerId player) {
        // Taker of Heads
        if (m_currentRecord.m_enemyModelsSlain > m_modelsSlainAtStartOfCombat) {
            PLOG_INFO.printf("%s slayed %d models this combat phase and heals 1 wound.\n",
                             name().c_str(), (m_currentRecord.m_enemyModelsSlain - m_modelsSlainAtStartOfCombat));
            heal(1);
        }
        KhorneBase::onEndCombat(player);
    }

    int Khorgoraths::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Khorgoraths::onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) {
        KhorneBase::onEnemyModelSlain(numSlain, enemyUnit, source);

        // Horrific Predators
        if ((numSlain > 0) && (enemyUnit != nullptr)) {
            enemyUnit->buffModifier(Attribute::Bravery, 1,
                                    {GamePhase::Battleshock, m_battleRound, m_currentRecord.m_playerWithTurn});
        }
    }

} //namespace Khorne

