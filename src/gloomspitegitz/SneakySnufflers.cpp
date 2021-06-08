/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SneakySnufflers.h>
#include <UnitFactory.h>
#include <Roster.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 6;
    static const int g_maxUnitSize = 18;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 210;

    bool SneakySnufflers::s_registered = false;

    SneakySnufflers::SneakySnufflers(Allegiance allegiance, int numModels, int points) :
            GloomspiteGitzBase(allegiance, "Sneaky Snufflers", 5, g_wounds, 4, 6, false, points),
            m_sickle(Weapon::Type::Melee, "Loonfungus Sickle", 1, 1, 4, 4, 0, 1),
            m_gnashers(Weapon::Type::Melee, "Gnashers", 1, 1, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, SNEAKY_SNUFFERS};
        m_weapons = {&m_sickle, &m_gnashers};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_sickle);
            model->addMeleeWeapon(&m_gnashers);
            addModel(model);
        }
    }

    Unit *SneakySnufflers::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new SneakySnufflers(allegiance, numModels, ComputePoints(parameters));
    }

    void SneakySnufflers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SneakySnufflers::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    SneakySnufflers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Sneaky Snufflers", factoryMethod);
        }
    }

    int SneakySnufflers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace GloomspiteGitz
