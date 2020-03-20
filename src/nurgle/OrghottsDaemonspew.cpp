/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/OrghottsDaemonspew.h"

namespace Nurgle {
    static const int BASESIZE = 100;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;

    bool OrghottsDaemonspew::s_registered = false;

    Unit *OrghottsDaemonspew::Create(const ParameterList &parameters) {
        auto unit = new OrghottsDaemonspew();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void OrghottsDaemonspew::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrghottsDaemonspew::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    OrghottsDaemonspew::ComputePoints,
                    {
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Orghotts Daemonspew", factoryMethod);
        }
    }

    OrghottsDaemonspew::OrghottsDaemonspew() :
            NurgleBase("Orghotts Daemonspew", 10, WOUNDS, 9, 3, false),
            m_tongue(Weapon::Type::Missile, "Whippermaw's Grasping Tongue", 6, 1, 3, 2, -1, RAND_D6),
            m_rotaxes(Weapon::Type::Melee, "The Rotaxes", 2, 5, 3, 3, -1, 1),
            m_claws(Weapon::Type::Melee, "Whippermaw's Monstrous Claws", 3, 5, 4, 2, -1, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, ORGHOTTS_DAEMONSPEW};
        m_weapons = {&m_tongue, &m_rotaxes, &m_claws};
    }

    bool OrghottsDaemonspew::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_tongue);
        model->addMeleeWeapon(&m_rotaxes);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int OrghottsDaemonspew::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // Fury of the Halfblood
        if (m_charged) extra += Dice::rollD3();

        return extra;
    }

    int OrghottsDaemonspew::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
