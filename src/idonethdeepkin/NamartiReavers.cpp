/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/NamartiReavers.h>
#include <Board.h>
#include <Roster.h>
#include <UnitFactory.h>
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 2;

    bool NamartiReavers::s_registered = false;

    NamartiReavers::NamartiReavers() :
            IdonethDeepkinBase("Namarti Reavers", 8, g_wounds, 6, 5, false),
            m_keeningBlade(Weapon::Type::Melee, "Keening Blade", 1, 2, 3, 4, 0, 1),
            m_whisperbowAimedFire(Weapon::Type::Missile, "Whisperbow: Aimed Fire", 18, 1, 4, 4, 0, 1),
            m_whisperbowStormFire(Weapon::Type::Missile, "Whisperbow: Storm Fire", 9, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, NAMARTI, REAVERS};
        m_weapons = {&m_keeningBlade, &m_whisperbowAimedFire, &m_whisperbowStormFire};
    }

    bool NamartiReavers::configure(int numModels, int numIconBearers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        if (numIconBearers > g_maxUnitSize / 10) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_whisperbowStormFire);
            model->addMissileWeapon(&m_whisperbowAimedFire);
            model->addMeleeWeapon(&m_keeningBlade);
            if (numIconBearers >= 0) {
                model->setName(Model::IconBearer);
                numIconBearers--;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *NamartiReavers::Create(const ParameterList &parameters) {
        auto unit = new NamartiReavers();
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

    void NamartiReavers::Init() {
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

            s_registered = UnitFactory::Register("Namarti Reavers", factoryMethod);
        }
    }

    void NamartiReavers::onStartShooting(PlayerId /*player*/) {
        auto board = Board::Instance();
        auto otherRoster = board->getPlayerRoster(GetEnemyId(owningPlayer()));

        // Fluid Firing Style
        auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
        if (nearestUnit) {
            double rangeTo = distanceTo(nearestUnit);
            if (rangeTo < (double) m_whisperbowStormFire.range()) {
                m_whisperbowStormFire.activate(true);
                m_whisperbowAimedFire.activate(false);
            } else {
                m_whisperbowStormFire.activate(false);
                m_whisperbowAimedFire.activate(true);
            }
        }
    }

    int NamartiReavers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace IdonethDeepkin
