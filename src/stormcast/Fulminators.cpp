/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Fulminators.h>
#include <UnitFactory.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 220;
    static const int g_pointsMaxUnitSize = 6 * g_pointsPerBlock;

    bool Fulminators::s_registered = false;

    Fulminators::Fulminators(Stormhost stormhost, int numModels) :
            StormcastEternal(stormhost, "Fulminators", 10, g_wounds, 7, 3, false),
            m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
            m_stormstrkeGlaive(Weapon::Type::Melee, "Stormstrike Glaive", 2, 3, 3, 3, -1, 1),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, FULMINATORS};
        m_weapons = {&m_stormBlast, &m_stormstrkeGlaive, &m_clawsAndFangs};
        m_hasMount = true;
        m_clawsAndFangs.setMount(true);

        for (int i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_stormBlast);
            model->addMeleeWeapon(&m_stormstrkeGlaive);
            model->addMeleeWeapon(&m_clawsAndFangs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Unit *Fulminators::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        return new Fulminators(stormhost, numModels);
    }

    void Fulminators::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Fulminators", factoryMethod);
        }
    }

    Wounds Fulminators::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Impaling Strike
        if (m_charged && (weapon->name() == m_clawsAndFangs.name())) {
            return {weapon->damage() + 2, 0};
        }

        // Intolerable Damage
        if ((hitRoll == 6) && (weapon->name() == m_clawsAndFangs.name())) {
            return {Dice::RollD6(), 0};
        }

        // Storm Blast
        if (weapon->name() == m_stormBlast.name()) {
            return {0, Dice::RollD3()};
        }

        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Rerolls Fulminators::toSaveRerolls(const Weapon * /*weapon*/, const Unit *attacker) const {
        // Sigmarite Shields
        return Rerolls::Ones;
    }

    int Fulminators::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = StormcastEternal::toSaveModifier(weapon, attacker);
        // Glaivewall
        if (weapon->isMissile()) {
            modifier += 1;
        }
        return modifier;
    }

    int Fulminators::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }
} //namespace StormcastEternals