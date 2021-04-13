/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/Decimators.h>
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

    bool Decimators::s_registered = false;

    Decimators::Decimators(Stormhost stormhost, int numModels, int numStarsoulMaces) :
            StormcastEternal(stormhost, "Decimators", 4, g_wounds, 7, 4, false),
            m_thunderaxe(Weapon::Type::Melee, "Thunderaxe", 2, 0, 3, 3, -1, 1),
            m_thunderaxePrime(Weapon::Type::Melee, "Thunderaxe", 2, 0, 3, 3, -1, 1),
            m_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 1, 0, 0, 0, 0) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, DECIMATORS};
        m_weapons = {&m_thunderaxe, &m_thunderaxePrime, &m_starsoulMace};

        s_globalBraveryMod.connect(this, &Decimators::grimHarvestors, &m_connection);

        // Add the Prime
        auto primeModel = new Model(g_basesize, wounds());
        primeModel->addMeleeWeapon(&m_thunderaxePrime);
        addModel(primeModel);

        for (auto i = 0; i < numStarsoulMaces; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_starsoulMace);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_thunderaxe);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Decimators::~Decimators() {
        m_connection.disconnect();
    }

    Wounds Decimators::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
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

    Unit *Decimators::Create(const ParameterList &parameters) {
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
        return new Decimators(stormhost, numModels, numStarsoulMaces);
    }

    void Decimators::Init() {
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

            s_registered = UnitFactory::Register("Decimators", factoryMethod);
        }
    }

    int Decimators::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = StormcastEternal::extraAttacks(nullptr, weapon, target);
        if (weapon->name() == m_thunderaxe.name()) {
            // count the number of models in the unit w/in 2 in of this model
            extra += target->numModelsWithin(attackingModel, (double) weapon->range());
        }
        return extra;
    }

    int Decimators::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Decimators::grimHarvestors(const Unit *target) {
        // Grim Harvestors
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 6.0)) {
            return -2;
        }

        return 0;
    }


} // namespace StormcastEternals
