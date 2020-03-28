/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/LoonsmashaFanatics.h>
#include <UnitFactory.h>

namespace GloomspiteGitz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    bool LoonsmashaFanatics::s_registered = false;

    Unit *LoonsmashaFanatics::Create(const ParameterList &parameters) {
        auto unit = new LoonsmashaFanatics();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int LoonsmashaFanatics::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void LoonsmashaFanatics::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LoonsmashaFanatics::Create,
                    nullptr,
                    nullptr,
                    LoonsmashaFanatics::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE}
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Loonsplatta Fanatics", factoryMethod);
        }
    }

    LoonsmashaFanatics::LoonsmashaFanatics() :
            GloomspiteGitzBase("Loonsplatta Fanatics", RAND_2D6, WOUNDS, 10, 6, false),
            m_ballAndChain(Weapon::Type::Melee, "Ball and Chain", 1, RAND_D6, 4, 3, -2, RAND_D3) {
        m_keywords = {DESTRUCTION, GLOOMSPITE_GITZ, MOONCLAN, FANATIC, LOONSMASHA};
        m_weapons = {&m_ballAndChain};
    }

    bool LoonsmashaFanatics::configure(int numModels) {
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