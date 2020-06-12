/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Raptoryx.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 6;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 450;

    bool Raptoryx::s_registered = false;

    Unit *Raptoryx::Create(const ParameterList &parameters) {
        auto unit = new Raptoryx();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Raptoryx::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Raptoryx::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Raptoryx::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Raptoryx", factoryMethod);
        }
    }

    Raptoryx::Raptoryx() :
            SlavesToDarknessBase("Raptoryx", 10, WOUNDS, 6, NoSave, true),
            m_beakAndTalons(Weapon::Type::Melee, "Razor-sharp Beak and Talons", 1, 2, 3, 3, 0, 1) {
        m_keywords = {CHAOS, SLAVES_TO_DARKNESS, RAPTORYX};
        m_weapons = {&m_beakAndTalons};
    }

    bool Raptoryx::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_beakAndTalons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Raptoryx::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int extra = Unit::extraAttacks(attackingModel, weapon, target);
        // Crazed Flock
        if (m_charged) {
            extra += 1;
        }
        return extra;
    }

    int Raptoryx::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //SlavesToDarkness