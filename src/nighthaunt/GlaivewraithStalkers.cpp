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
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 16;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool GlaivewraithStalkers::s_registered = false;

    GlaivewraithStalkers::GlaivewraithStalkers() :
            Nighthaunt("Glaivewraith Stalkers", 6, WOUNDS, 10, 4, true),
            m_huntersGlaive(Weapon::Type::Melee, "Hunter's Glaive", 2, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, GLAIVEWRAITH_STALKERS};
        m_weapons = {&m_huntersGlaive};
    }

    bool GlaivewraithStalkers::configure(int numModels, bool drummer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_drummer = drummer;
        m_runAndCharge = m_drummer;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_huntersGlaive);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *GlaivewraithStalkers::Create(const ParameterList &parameters) {
        auto unit = new GlaivewraithStalkers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
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
                    nullptr,
                    nullptr,
                    GlaivewraithStalkers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
            return RerollFailed;
        }

        return Nighthaunt::toHitRerolls(weapon, unit);
    }

    int GlaivewraithStalkers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Nighthaunt
