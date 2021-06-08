/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/SporesplattaFanatics.h>
#include <UnitFactory.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool SporesmashaFanatics::s_registered = false;

    Unit *SporesmashaFanatics::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new SporesmashaFanatics(allegiance, numModels, ComputePoints(parameters));
    }

    int SporesmashaFanatics::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void SporesmashaFanatics::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SporesmashaFanatics::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    SporesmashaFanatics::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Sporesplatta Fanatics", factoryMethod);
        }
    }

    SporesmashaFanatics::SporesmashaFanatics(Allegiance allegiance, int numModels, int points) :
            GloomspiteGitzBase(allegiance, "Sporesplatta Fanatics", RAND_2D6, g_wounds, 10, 6, false, points),
            m_ballAndChain(Weapon::Type::Melee, "Spore-ball and Chain", 1, RAND_D3, 2, 4, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GLOOMSPITE_GITZ, MOONCLAN, FANATIC, SPORESPLATTA};
        m_weapons = {&m_ballAndChain};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_ballAndChain);
            addModel(model);
        }
    }

} // namespace GloomspiteGitz
