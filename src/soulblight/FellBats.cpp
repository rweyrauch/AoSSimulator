/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/FellBats.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 9;
    static const int g_pointsPerBlock = 75;
    static const int g_pointsMaxUnitSize = 3 * g_pointsPerBlock;

    bool FellBats::s_registered = false;

    Unit *FellBats::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new FellBats(bloodline, numModels, ComputePoints(parameters));
    }

    int FellBats::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void FellBats::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Fell Bats", factoryMethod);
        }
    }

    FellBats::FellBats(CursedBloodline bloodline, int numModels, int points) :
            SoulblightBase(bloodline, "Fell Bats", 14, g_wounds, 10, 6, true, points),
            m_fangs(Weapon::Type::Melee, "Elongated Fangs", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, SUMMONABLE, FELL_BATS};
        m_weapons = {&m_fangs};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_fangs);
            addModel(model);
        }
    }
} // namespace Soulblight
