/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/ChaosSpawn.h>
#include <UnitFactory.h>

namespace BeastsOfChaos {
    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 50;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    bool ChaosSpawn::s_registered = false;

    ChaosSpawn::ChaosSpawn() :
            BeastsOfChaosBase("Chaos Spawn", RAND_2D6, WOUNDS, 10, 5, false),
            m_freakingMutations(Weapon::Type::Melee, "Freakish Mutations", 1, RAND_2D6, 4, 4, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MORTAL, SLAVES_TO_DARKNESS, CHAOS_SPAWN};
        m_weapons = {&m_freakingMutations};
    }

    bool ChaosSpawn::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_freakingMutations);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *ChaosSpawn::Create(const ParameterList &parameters) {
        auto unit = new ChaosSpawn();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosSpawn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };

            s_registered = UnitFactory::Register("Chaos Spawn", factoryMethod);
        }
    }

    int ChaosSpawn::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace BeastsOfChaos