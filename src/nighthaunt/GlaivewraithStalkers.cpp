/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/GlaivewraithStalkers.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 4;
    static const int g_maxUnitSize = 16;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 320;

    bool GlaivewraithStalkers::s_registered = false;

    GlaivewraithStalkers::GlaivewraithStalkers() :
            Nighthaunt("Glaivewraith Stalkers", 6, g_wounds, 10, 4, true),
            m_huntersGlaive(Weapon::Type::Melee, "Hunter's Glaive", 2, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, GLAIVEWRAITH_STALKERS};
        m_weapons = {&m_huntersGlaive};
    }

    bool GlaivewraithStalkers::configure(int numModels, bool drummer) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_runAndCharge = drummer;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_huntersGlaive);
            if (drummer) {
                model->setName(Model::Drummer);
                drummer = false;
            }

            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *GlaivewraithStalkers::Create(const ParameterList &parameters) {
        auto unit = new GlaivewraithStalkers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool drummer = GetBoolParam("Drummer", parameters, true);

        bool ok = unit->configure(numModels, drummer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GlaivewraithStalkers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GlaivewraithStalkers::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    GlaivewraithStalkers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Drummer")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Glaivewraith Stalkers", factoryMethod);
        }
    }

    Rerolls GlaivewraithStalkers::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // The Point of Death
        if (m_charged || unit->charged()) {
            return Reroll_Failed;
        }

        return Nighthaunt::toHitRerolls(weapon, unit);
    }

    int GlaivewraithStalkers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nighthaunt
