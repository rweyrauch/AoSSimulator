/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarplockJezzails.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 560;

    bool WarplockJezzails::s_registered = false;

    Unit *WarplockJezzails::Create(const ParameterList &parameters) {
        auto unit = new WarplockJezzails();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int WarplockJezzails::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void WarplockJezzails::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Warplock Jezzails", factoryMethod);
        }
    }

    WarplockJezzails::WarplockJezzails() :
            Skaventide("Warplock Jezzails", 6, g_wounds, 4, 6, false),
            m_jezzail(Weapon::Type::Missile, "Warplock Jezzail", 30, 1, 4, 3, -2, 2),
            m_knives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WARPLOCK_JEZZAILS};
        m_weapons = {&m_jezzail, &m_knives};
        m_battleFieldRole = Artillery;
    }

    bool WarplockJezzails::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_jezzail);
            model->addMeleeWeapon(&m_knives);
            addModel(model);
        }
        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds WarplockJezzails::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Warpstone Snipers
        if ((hitRoll == 6) && (weapon->name() == m_jezzail.name())) {
            return {0, 2};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls WarplockJezzails::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Pavise
        if (!m_moved && (weapon->name() == m_jezzail.name())) {
            return Reroll_Failed;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int WarplockJezzails::toSaveModifier(const Weapon *weapon) const {
        auto mod = Unit::toSaveModifier(weapon);

        // Pavise
        if (weapon->isMissile()) {
            mod += 2;
        }

        return mod;
    }

} //namespace Skaven
