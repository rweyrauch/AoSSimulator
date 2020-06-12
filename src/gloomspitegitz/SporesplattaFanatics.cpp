/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/SporesplattaFanatics.h>
#include <UnitFactory.h>

namespace GloomspiteGitz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool SporesmashaFanatics::s_registered = false;

    Unit *SporesmashaFanatics::Create(const ParameterList &parameters) {
        auto unit = new SporesmashaFanatics();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int SporesmashaFanatics::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE)
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Sporesplatta Fanatics", factoryMethod);
        }
    }

    SporesmashaFanatics::SporesmashaFanatics() :
            GloomspiteGitzBase("Sporesplatta Fanatics", RAND_2D6, WOUNDS, 10, 6, false),
            m_ballAndChain(Weapon::Type::Melee, "Spore-ball and Chain", 1, RAND_D3, 2, 4, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GLOOMSPITE_GITZ, MOONCLAN, FANATIC, SPORESPLATTA};
        m_weapons = {&m_ballAndChain};
    }

    bool SporesmashaFanatics::configure(int numModels) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_ballAndChain);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

} // namespace GloomspiteGitz
