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
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool SquiqHoppers::s_registered = false;

    SquiqHoppers::SquiqHoppers() :
            GloomspiteGitzBase("Squig Hoppers", RAND_3D6, WOUNDS, 4, 6, true),
            m_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1),
            m_slitta(Weapon::Type::Melee, "Slitta", 1, 1, 5, 5, 0, 1),
            m_slittaBoss(Weapon::Type::Melee, "Slitta", 1, 1, 4, 5, 0, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HOPPERS};
        m_weapons = {&m_fangFilledGob, &m_slitta, &m_slittaBoss};
    }

    bool SquiqHoppers::configure(int numModels) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        // Add the boss
        auto boss = new Model(BASESIZE, wounds());
        boss->addMeleeWeapon(&m_slittaBoss);
        boss->addMeleeWeapon(&m_fangFilledGob);
        addModel(boss);

        // and the rest
        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_slitta);
            model->addMeleeWeapon(&m_fangFilledGob);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SquiqHoppers::Create(const ParameterList &parameters) {
        auto unit = new SquiqHoppers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
                    nullptr,
                    nullptr,
                    SquiqHoppers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE)
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Squiq Hoppers", factoryMethod);
        }
    }

    int SquiqHoppers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace GloomspiteGitz
