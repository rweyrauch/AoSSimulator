/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SquigHoppers.h>
#include <UnitFactory.h>

namespace GloomspiteGitz {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 360;

    bool SquiqHoppers::s_registered = false;

    SquiqHoppers::SquiqHoppers() :
            GloomspiteGitzBase("Squig Hoppers", RAND_3D6, g_wounds, 4, 6, true),
            m_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1),
            m_slitta(Weapon::Type::Melee, "Slitta", 1, 1, 5, 5, 0, 1),
            m_slittaBoss(Weapon::Type::Melee, "Slitta", 1, 1, 4, 5, 0, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HOPPERS};
        m_weapons = {&m_fangFilledGob, &m_slitta, &m_slittaBoss};
        m_hasMount = true;
        m_fangFilledGob.setMount(true);
    }

    bool SquiqHoppers::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the boss
        auto boss = new Model(g_basesize, wounds());
        boss->addMeleeWeapon(&m_slittaBoss);
        boss->addMeleeWeapon(&m_fangFilledGob);
        addModel(boss);

        // and the rest
        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_slitta);
            model->addMeleeWeapon(&m_fangFilledGob);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SquiqHoppers::Create(const ParameterList &parameters) {
        auto unit = new SquiqHoppers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SquiqHoppers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SquiqHoppers::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    SquiqHoppers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize)
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Squiq Hoppers", factoryMethod);
        }
    }

    int SquiqHoppers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace GloomspiteGitz
