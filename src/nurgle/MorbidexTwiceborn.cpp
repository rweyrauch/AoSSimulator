/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/MorbidexTwiceborn.h"

namespace Nurgle {
    static const int BASESIZE = 100;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;

    bool MorbidexTwiceborn::s_registered = false;

    Unit *MorbidexTwiceborn::Create(const ParameterList &parameters) {
        auto unit = new MorbidexTwiceborn();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MorbidexTwiceborn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MorbidexTwiceborn::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    MorbidexTwiceborn::ComputePoints,
                    {
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Morbidex Twiceborn", factoryMethod);
        }
    }

    MorbidexTwiceborn::MorbidexTwiceborn() :
            NurgleBase("Morbidex Twicebornd", 10, WOUNDS, 9, 3, false),
            m_tongues(Weapon::Type::Missile, "Slabrous Tongues", 6, 3, 3, 2, -1, 1),
            m_scythe(Weapon::Type::Melee, "Fleshreaper Scythe", 2, 5, 3, 3, -1, 2),
            m_claws(Weapon::Type::Melee, "Monstrous Claws", 3, 5, 4, 2, -1, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, MORBIDEX_TWICEBORN};
        m_weapons = {&m_tongues, &m_scythe, &m_claws};

        s_globalToWoundMod.connect(this, &MorbidexTwiceborn::maliciousMitesWoundMod, &m_maliciousMitesSlot);
    }

    MorbidexTwiceborn::~MorbidexTwiceborn() {
        m_maliciousMitesSlot.disconnect();
    }

    bool MorbidexTwiceborn::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_tongues);
        model->addMeleeWeapon(&m_scythe);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int MorbidexTwiceborn::maliciousMitesWoundMod(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (isFriendly(attacker) && attacker->hasKeyword(NURGLINGS) && (distanceTo(attacker) <= 7.0f)) return 1;
        return 0;
    }

    int MorbidexTwiceborn::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
