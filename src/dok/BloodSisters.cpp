/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/BloodSisters.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine {
    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    bool BloodSisters::s_registered = false;

    BloodSisters::BloodSisters() :
            DaughterOfKhaine("Blood Sisters", 8, WOUNDS, 8, 5, false),
            m_heartshardGlaive(Weapon::Type::Melee, "Heartshard Glaive", 2, 3, 3, 3, -1, 1),
            m_heartshardGlaiveGorgai(Weapon::Type::Melee, "Heartshard Glaive", 2, 3, 2, 3, -1, 1),
            m_crystalTouch(Weapon::Type::Melee, "Crystal Touch", 1, 1, 4, 0, 0, 1),
            m_crystalTouchGorgai(Weapon::Type::Melee, "Crystal Touch", 1, 1, 3, 0, 0, 1) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, MELUSAI, BLOOD_SISTERS};
        m_weapons = {&m_heartshardGlaive, &m_heartshardGlaiveGorgai, &m_crystalTouch, &m_crystalTouchGorgai};
    }

    bool BloodSisters::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto gorgai = new Model(BASESIZE, wounds());
        gorgai->addMeleeWeapon(&m_heartshardGlaiveGorgai);
        gorgai->addMeleeWeapon(&m_crystalTouchGorgai);
        addModel(gorgai);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_heartshardGlaive);
            model->addMeleeWeapon(&m_crystalTouch);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BloodSisters::Create(const ParameterList &parameters) {
        auto unit = new BloodSisters();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BloodSisters::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Blood Sisters", factoryMethod);
        }
    }

    Wounds BloodSisters::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Turned to Crystal
        if (weapon->name() == m_crystalTouch.name()) {
            return {0, 1};
        }

        return DaughterOfKhaine::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BloodSisters::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace DaughtersOfKhaine