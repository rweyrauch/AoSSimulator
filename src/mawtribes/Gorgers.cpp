/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Gorgers.h"

namespace OgorMawtribes {
    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    bool Gorgers::s_registered = false;

    Unit *Gorgers::Create(const ParameterList &parameters) {
        auto unit = new Gorgers();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, None);
        unit->setMawtribe(tribe);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }


    void Gorgers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Gorgers::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    Gorgers::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None,
                             MawtribesBase::Winterbite, 1}
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Gorgers", factoryMethod);
        }
    }

    Gorgers::Gorgers() :
            MawtribesBase("Gorgers", 6, WOUNDS, 8, 6, false),
            m_claws(Weapon::Type::Melee, "Long Claws", 1, 4, 3, 3, 0, 2),
            m_jaw(Weapon::Type::Melee, "Distensible Jaw", 1, 1, 3, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, GORGERS};
        m_weapons = {&m_claws, &m_jaw};
    }

    bool Gorgers::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_claws);
            model->addMeleeWeapon(&m_jaw);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Gorgers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace OgorMawtribes
