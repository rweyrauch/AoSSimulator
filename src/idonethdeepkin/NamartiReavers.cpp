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
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 2;

    bool NamartiReavers::s_registered = false;

    NamartiReavers::NamartiReavers() :
            IdonethDeepkinBase("Namarti Reavers", 8, WOUNDS, 6, 5, false),
            m_keeningBlade(Weapon::Type::Melee, "Keening Blade", 1, 2, 3, 4, 0, 1),
            m_whisperbowAimedFire(Weapon::Type::Missile, "Whisperbow: Aimed Fire", 18, 1, 4, 4, 0, 1),
            m_whisperbowStormFire(Weapon::Type::Missile, "Whisperbow: Storm Fire", 9, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, NAMARTI, REAVERS};
        m_weapons = {&m_keeningBlade, &m_whisperbowAimedFire, &m_whisperbowStormFire};
    }

    bool NamartiReavers::configure(int numModels, int numIconBearers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        if (numIconBearers > MAX_UNIT_SIZE / 10) {
            return false;
        }

        m_numIconBearers = numIconBearers;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_whisperbowStormFire);
            model->addMissileWeapon(&m_whisperbowAimedFire);
            model->addMeleeWeapon(&m_keeningBlade);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *NamartiReavers::Create(const ParameterList &parameters) {
        auto unit = new NamartiReavers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            IntegerParameter("Icon Bearers", 0, 0, MAX_UNIT_SIZE / 10, 1),
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace IdonethDeepkin
