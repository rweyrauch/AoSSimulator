/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Desolators.h>
#include <UnitFactory.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = 6 * g_pointsPerBlock;

    bool Desolators::s_registered = false;

    Desolators::Desolators(Stormhost stormhost, int numModels, int points) :
            StormcastEternal(stormhost, "Desolators", 10, g_wounds, 7, 3, false, points),
            m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
            m_thunderAxe(Weapon::Type::Melee, "Thunderaxe", 2, 3, 4, 3, -1, 2),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, DESOLATORS};
        m_weapons = {&m_stormBlast, &m_thunderAxe, &m_clawsAndFangs};
        m_hasMount = true;
        m_clawsAndFangs.setMount(true);

        for (int i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_stormBlast);
            model->addMeleeWeapon(&m_thunderAxe);
            model->addMeleeWeapon(&m_clawsAndFangs);
            addModel(model);
        }
    }

    Unit *Desolators::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        return new Desolators(stormhost, numModels, ComputePoints(parameters));
    }

    void Desolators::Init() {
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

            s_registered = UnitFactory::Register("Desolators", factoryMethod);
        }
    }

    Wounds Desolators::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Intolerable Damage
        if ((hitRoll == 6) && (weapon->name() == m_clawsAndFangs.name())) {
            return {Dice::RollD6(), 0};
        }

        // Storm Blast
        if (weapon->name() == m_stormBlast.name()) {
            return {0, Dice::RollD3(), Wounds::Source::Weapon_Melee, weapon};
        }

        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Rerolls Desolators::toSaveRerolls(const Weapon */*weapon*/, const Unit *attacker) const {
        // Sigmarite Shields
        return Rerolls::Ones;
    }

    int Desolators::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);
        if (weapon->name() == m_thunderAxe.name()) {
            if (remainingModels() >= 6)
                attacks += 2;
            else if (remainingModels() >= 4)
                attacks += 1;
        }
        return attacks;
    }

    int Desolators::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace StormcastEternals