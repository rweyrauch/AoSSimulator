/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "tzeentch/TheEyesOfNine.h"

namespace Tzeentch {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    bool TheEyesOfTheNine::s_registered = false;

    Unit *TheEyesOfTheNine::Create(const ParameterList &parameters) {
        auto unit = new TheEyesOfTheNine();

        unit->setChangeCoven(CultOfTheTransientForm);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int TheEyesOfTheNine::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void TheEyesOfTheNine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheEyesOfTheNine::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TheEyesOfTheNine::ComputePoints,
                    {
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("The Eyes of the Nine", factoryMethod);
        }
    }

    TheEyesOfTheNine::TheEyesOfTheNine() {

    }

    bool TheEyesOfTheNine::configure() {
        auto narvia = new Model(BASESIZE, wounds());
        narvia->addMissileWeapon(&m_bolt);
        narvia->addMeleeWeapon(&m_blade);
        narvia->setName("Narvia");
        addModel(narvia);

        auto turosh = new Model(BASESIZE, wounds());
        turosh->addMissileWeapon(&m_bolt);
        turosh->addMeleeWeapon(&m_blade);
        turosh->setName("Turosh");
        addModel(turosh);

        auto kcharik = new Model(BASESIZE, wounds());
        kcharik->addMeleeWeapon(&m_greatblade);
        kcharik->addMeleeWeapon(&m_beak);
        kcharik->setName("K'charik");
        addModel(kcharik);

        auto horror = new Model(BASESIZE, wounds());
        horror->addMissileWeapon(&m_flames);
        horror->addMeleeWeapon(&m_hands);
        horror->setName("Blue Horror");
        addModel(horror);

        m_points = POINTS_PER_UNIT;

        return true;
    }
} // namespace Tzeentch


