/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/KhainiteShadowstalkers.h>
#include <UnitFactory.h>
#include <iostream>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 9;
    static const int g_maxUnitSize = 18;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool KhainiteShadowstalkers::s_registered = false;

    KhainiteShadowstalkers::KhainiteShadowstalkers() :
            DaughterOfKhaine("Khainite Shadowstalkers", 6, g_wounds, 7, 5, false),
            m_cursedMissiles(Weapon::Type::Missile, "Cursed Missiles", 6, 1, 4, 3, 0, 1),
            m_assassinsBlades(Weapon::Type::Melee, "Assassin's Blades", 1, 2, 4, 3, 0, 1),
            m_umbralBlades(Weapon::Type::Melee, "Umbral Blades", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, KHAINITE_SHADOWSTALKERS};
        m_weapons = {&m_cursedMissiles, &m_assassinsBlades, &m_umbralBlades};
    }

    bool KhainiteShadowstalkers::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto queen = new Model(g_basesize, wounds() + 2);
        queen->addMissileWeapon(&m_cursedMissiles);
        queen->addMeleeWeapon(&m_umbralBlades);
        queen->setName("Shroud Queen");
        addModel(queen);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_cursedMissiles);
            model->addMeleeWeapon(&m_assassinsBlades);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *KhainiteShadowstalkers::Create(const ParameterList &parameters) {
        auto unit = new KhainiteShadowstalkers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto temple = (Temple) GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KhainiteShadowstalkers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Khainite Shadowstalkers", factoryMethod);
        }
    }

    Wounds
    KhainiteShadowstalkers::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Cursed Missiles
        if ((hitRoll == 6) && (weapon->name() == m_cursedMissiles.name())) {
            return {0, 1};
        }

        return DaughterOfKhaine::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int KhainiteShadowstalkers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int KhainiteShadowstalkers::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = DaughterOfKhaine::targetHitModifier(weapon, attacker);

        // Harness Shadow
        if (!weapon->isMissile()) mod--;

        return mod;
    }

} // namespace DaughtersOfKhaine