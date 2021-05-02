/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/Protectors.h>
#include <UnitFactory.h>
#include <iostream>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 170;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Protectors::s_registered = false;

    Protectors::Protectors(Stormhost stormhost, int numModels, int numStarsoulMaces, int points) :
            StormcastEternal(stormhost, "Protectors", 4, g_wounds, 7, 4, false, points),
            m_glaive(Weapon::Type::Melee, "Stormstrike Glaive", 3, 3, 3, 3, -1, 1),
            m_glaivePrime(Weapon::Type::Melee, "Stormstrike Glaive", 3, 4, 3, 3, -1, 1),
            m_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 1, 0, 0, 0, 0) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, PROTECTORS};
        m_weapons = {&m_glaive, &m_glaivePrime, &m_starsoulMace};

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        primeModel->addMeleeWeapon(&m_glaivePrime);
        addModel(primeModel);

        for (auto i = 0; i < numStarsoulMaces; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_starsoulMace);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_glaive);
            addModel(model);
        }
    }

    Wounds Protectors::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deathstrike
        if ((hitRoll == 6) && target->hasKeyword(MONSTER) && (weapon->name() == m_glaive.name())) {
            return {Dice::RollD6(), 0};
        }
        // Starsoul Mace
        if (weapon->name() == m_starsoulMace.name()) {
            int mortalWounds = 0;
            int roll = Dice::RollD6();
            if (roll >= 6) {
                mortalWounds = Dice::RollD3() + 1;
            } else if (roll >= 2) {
                mortalWounds = Dice::RollD3();
            }
            return {0, mortalWounds};
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Unit *Protectors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numStarsoulMaces = GetIntParam("Starsoul Maces", parameters, 0);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);

        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return nullptr;
        }
        const int maxStarsoulMaces = (numModels / 5) * 2;
        if (numStarsoulMaces > maxStarsoulMaces) {
            // Invalid weapon configuration.
            return nullptr;
        }
        return new Protectors(stormhost, numModels, numStarsoulMaces, ComputePoints(parameters));
    }

    void Protectors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Starsoul Maces", 2, 0, (g_maxUnitSize / 5) * 2, 1),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Protectors", factoryMethod);
        }
    }

    int Protectors::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = StormcastEternal::targetHitModifier(weapon, attacker);

        // Storm-shield
        if (weapon->isMissile()) {
            modifier -= 1;
        }
        return modifier;
    }

    int Protectors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace StormcastEternals
