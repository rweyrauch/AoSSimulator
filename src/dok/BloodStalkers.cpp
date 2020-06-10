/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/BloodStalkers.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine {
    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 640;

    bool BloodStalkers::s_registered = false;

    BloodStalkers::BloodStalkers() :
            DaughterOfKhaine("Blood Stalkers", 8, WOUNDS, 8, 5, false),
            m_hearseekerBow(Weapon::Type::Missile, "Heartseeker Bow", 24, 1, 3, 3, -1, 1),
            m_sacraficialDaggar(Weapon::Type::Melee, "Sacrificial Dagger", 1, 2, 3, 4, 0, 1),
            m_heartseekBowKrone(Weapon::Type::Missile, "Heartseeker Bow", 24, 1, 2, 3, -1, 1),
            m_sacraficialDaggarKrone(Weapon::Type::Melee, "Sacrificial Dagger", 1, 2, 2, 4, 0, 1),
            m_bloodWyrm(Weapon::Type::Melee, "Blood Wyrm", 1, 1, 3, 3, 0, 1) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, MELUSAI, BLOOD_STALKERS};
        m_weapons = {&m_hearseekerBow, &m_sacraficialDaggar, &m_heartseekBowKrone, &m_sacraficialDaggarKrone,
                     &m_bloodWyrm};
    }

    bool BloodStalkers::configure(int numModels, bool bloodWyrm) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto krone = new Model(BASESIZE, wounds());
        krone->addMissileWeapon(&m_heartseekBowKrone);
        krone->addMeleeWeapon(&m_sacraficialDaggarKrone);
        if (bloodWyrm) {
            krone->addMeleeWeapon(&m_bloodWyrm);
        }
        addModel(krone);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_hearseekerBow);
            model->addMeleeWeapon(&m_sacraficialDaggar);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BloodStalkers::Create(const ParameterList &parameters) {
        auto unit = new BloodStalkers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool bloodWyrm = GetBoolParam("Blood Wyrm", parameters, false);

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        bool ok = unit->configure(numModels, bloodWyrm);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BloodStalkers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Blood Wyrm"),
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Blood Stalkers", factoryMethod);
        }
    }

    Wounds BloodStalkers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Heartseekers
        if ((hitRoll >= 6) && (weapon->name() == m_hearseekerBow.name())) {
            return {0, 1};
        }

        return DaughterOfKhaine::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BloodStalkers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace DaughtersOfKhaine