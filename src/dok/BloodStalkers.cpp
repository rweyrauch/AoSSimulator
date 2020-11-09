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
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 40;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool BloodStalkers::s_registered = false;

    BloodStalkers::BloodStalkers() :
            DaughterOfKhaine("Blood Stalkers", 8, g_wounds, 8, 5, false),
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
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto krone = new Model(g_basesize, wounds());
        krone->addMissileWeapon(&m_heartseekBowKrone);
        krone->addMeleeWeapon(&m_sacraficialDaggarKrone);
        if (bloodWyrm) {
            krone->addMeleeWeapon(&m_bloodWyrm);
        }
        addModel(krone);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_hearseekerBow);
            model->addMeleeWeapon(&m_sacraficialDaggar);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BloodStalkers::Create(const ParameterList &parameters) {
        auto unit = new BloodStalkers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace DaughtersOfKhaine