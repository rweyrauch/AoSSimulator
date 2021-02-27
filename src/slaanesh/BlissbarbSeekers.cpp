/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/BlissbarbSeekers.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 4;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize/g_minUnitSize)*g_pointsPerBlock;

    bool BlissbarbSeekers::s_registered = false;

    BlissbarbSeekers::BlissbarbSeekers() :
            SlaaneshBase("Blissbarb Seekers", 14, g_wounds, 6, 5, false),
            m_bow(Weapon::Type::Missile, "Blissbarb Bow", 18, 3, 4, 4, -1, 1),
            m_bowTempter(Weapon::Type::Missile, "Blissbarb Bow", 18, 4, 4, 4, -1, 1),
            m_blade(Weapon::Type::Melee, "Sybarite Blade", 1, 1, 4, 4, 0, 1),
            m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, SEEKERS, BLISSBARB_SEEKERS};
        m_weapons = {&m_bow, &m_bowTempter, &m_blade, &m_poisonedTongue};
        m_hasMount = true;
        m_poisonedTongue.setMount(true);
        // Flawless Accuracy
        m_runAndCharge = true;
    }

    bool BlissbarbSeekers::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // Add the High Tempter
        auto tempter = new Model(g_basesize, wounds());
        tempter->addMissileWeapon(&m_bowTempter);
        tempter->addMeleeWeapon(&m_poisonedTongue);
        tempter->addMeleeWeapon(&m_blade);
        tempter->setName("High Tempter");
        addModel(tempter);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_bow);
            model->addMeleeWeapon(&m_poisonedTongue);
            model->addMeleeWeapon(&m_blade);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BlissbarbSeekers::Create(const ParameterList &parameters) {
        auto unit = new BlissbarbSeekers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BlissbarbSeekers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Blissbarb Seekers", factoryMethod);
        }
    }

    int BlissbarbSeekers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds BlissbarbSeekers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Vectors of Agony
        if ((woundRoll == 6) && weapon->isMissile()) {
            return {0, 1};
        }
        return UnitModifierInterface::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

} // namespace Slaanesh