/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/GrimghastReapers.h>
#include <UnitFactory.h>
#include <iostream>
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = 420;

    bool GrimghastReapers::s_registered = false;

    GrimghastReapers::GrimghastReapers(Procession procession, int numModels, int points) :
            Nighthaunt(procession, "Grimghast Reapers", 8, g_wounds, 10, 4, true, points),
            m_slasherScythe(Weapon::Type::Melee, "Slasher Scythe", 2, 2, 4, 3, -1, 1),
            m_deathKnell(Weapon::Type::Melee, "Death Knell", 2, 1, 3, 3, -1, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, GRIMGHAST_REAPERS};
        m_weapons = {&m_slasherScythe, &m_deathKnell};

        auto extoller = new Model(g_basesize, wounds());
        // TODO: implement 'For Whom the Bell Tolls' (mortal wounds)
        extoller->addMeleeWeapon(&m_deathKnell);
        addModel(extoller);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_slasherScythe);
            addModel(model);
        }
    }

    Unit *GrimghastReapers::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new GrimghastReapers(procession, numModels, ComputePoints(parameters));
    }

    void GrimghastReapers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GrimghastReapers::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    GrimghastReapers::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Grimghast Reapers", factoryMethod);
        }
    }

    Rerolls GrimghastReapers::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Reaped Like Corn
        if (weapon->name() == "Slasher Scythe" && unit->remainingModels() >= 5) {
            return Rerolls::Failed;
        }

        return Nighthaunt::toHitRerolls(weapon, unit);
    }

    int GrimghastReapers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nighthaunt
