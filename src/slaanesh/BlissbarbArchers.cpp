/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/BlissbarbArchers.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 11;
    static const int g_maxUnitSize = 33;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool BlissbarbArchers::s_registered = false;

    BlissbarbArchers::BlissbarbArchers() :
            SlaaneshBase("Blissbarb Archers", 6, g_wounds, 6, 6, false),
            m_bow(Weapon::Type::Missile, "Blissbarb Bow", 18, 2, 4, 4, -1, 1),
            m_bowTempter(Weapon::Type::Missile, "Blissbarb Bow", 18, 3, 4, 4, -1, 1),
            m_blade(Weapon::Type::Melee, "Sybarite Blade", 1, 1, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, BLISSBARB_ARCHERS};
        m_weapons = {&m_bow, &m_bowTempter, &m_blade};

        // Light-footed Killers
        m_runAndCharge = true;
    }

    bool BlissbarbArchers::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // Add the High Tempter
        auto tempter = new Model(g_basesize, wounds());
        tempter->addMissileWeapon(&m_bowTempter);
        tempter->setName("High Tempter");
        addModel(tempter);

        auto homonculus = new Model(g_basesize, wounds());
        homonculus->addMeleeWeapon(&m_blade);
        homonculus->setName("Blissbrew Homonculus");
        addModel(homonculus);

        for (auto i = 2; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_bow);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BlissbarbArchers::Create(const ParameterList &parameters) {
        auto unit = new BlissbarbArchers();
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

    void BlissbarbArchers::Init() {
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
            s_registered = UnitFactory::Register("Blissbarb Archers", factoryMethod);
        }
    }

    int BlissbarbArchers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int BlissbarbArchers::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SlaaneshBase::toWoundModifier(weapon, target);

        if (weapon->isMissile() && isNamedModelAlive("Blissbrew Homonculus")) {
            mod++;
        }
        return mod;
    }

} // namespace Slaanesh