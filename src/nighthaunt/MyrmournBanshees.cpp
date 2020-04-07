/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/MyrmournBanshees.h"

namespace Nighthaunt {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 210;

    bool MyrmournBanshees::s_registered = false;

    Unit *MyrmournBanshees::Create(const ParameterList &parameters) {
        auto unit = new MyrmournBanshees();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int MyrmournBanshees::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void MyrmournBanshees::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MyrmournBanshees::Create,
                    nullptr,
                    nullptr,
                    MyrmournBanshees::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Myrmourn Banshees", factoryMethod);
        }
    }

    MyrmournBanshees::MyrmournBanshees() :
            Nighthaunt("Myrmourn Banshees", 8, WOUNDS, 10, 4, true),
            m_dagger(Weapon::Type::Melee, "Chill Dagger", 1, 1, 4, 3, -2, RAND_D3) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, MYRMOURN_BANSHEES};
        m_weapons = {&m_dagger};
    }

    bool MyrmournBanshees::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_dagger);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

} // namespace Nighthaunt

