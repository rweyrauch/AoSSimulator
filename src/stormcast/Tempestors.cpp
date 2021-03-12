/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Tempestors.h>
#include <UnitFactory.h>
#include <Board.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 6;

    bool Tempestors::s_registered = false;

    Tempestors::Tempestors() :
            StormcastEternal("Tempestors", 10, g_wounds, 7, 3, false),
            m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
            m_volleystormCrossbow(Weapon::Type::Missile, "Volleystorm Crossbow", 12, 4, 3, 4, 0, 1),
            m_warblade(Weapon::Type::Melee, "Warblade", 1, 3, 3, 4, 0, 1),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, TEMPESTORS};
        m_weapons = {&m_stormBlast, &m_volleystormCrossbow, &m_warblade, &m_clawsAndFangs};
        m_hasMount = true;
        m_clawsAndFangs.setMount(true);
    }

    bool Tempestors::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (int i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_stormBlast);
            model->addMissileWeapon(&m_volleystormCrossbow);
            model->addMeleeWeapon(&m_warblade);
            model->addMeleeWeapon(&m_clawsAndFangs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Tempestors::Create(const ParameterList &parameters) {
        auto unit = new Tempestors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Tempestors::Init() {
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

            s_registered = UnitFactory::Register("Tempestors", factoryMethod);
        }
    }

    Wounds Tempestors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Intolerable Damage
        if ((hitRoll == 6) && (weapon->name() == m_clawsAndFangs.name())) {
            return {Dice::RollD6(), 0};
        }

        // Storm Blast
        if (weapon->name() == m_stormBlast.name()) {
            return {0, Dice::RollD3()};
        }

        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls Tempestors::toSaveRerolls(const Weapon * /*weapon*/, const Unit *attacker) const {
        // Sigmarite Shields
        return Rerolls::Ones;
    }

    int Tempestors::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = StormcastEternal::targetHitModifier(weapon, attacker);

        // Disruptive Fire
        if (weapon->isMissile()) {
            auto unit = Board::Instance()->getUnitWithKeyword(attacker, owningPlayer(), TEMPESTORS, 12);
            if (unit != nullptr)
                modifier -= 1;
        }
        return modifier;
    }

    int Tempestors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }
} //namespace StormcastEternals