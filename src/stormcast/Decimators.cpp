/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Decimators.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    bool Decimators::s_registered = false;

    Decimators::Decimators() :
            StormcastEternal("Decimators", 4, WOUNDS, 7, 4, false),
            m_thunderaxe(Weapon::Type::Melee, "Thunderaxe", 2, 0, 3, 3, -1, 1),
            m_thunderaxePrime(Weapon::Type::Melee, "Thunderaxe", 2, 0, 3, 3, -1, 1),
            m_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 1, 0, 0, 0, 0) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, DECIMATORS};
        m_weapons = {&m_thunderaxe, &m_thunderaxePrime, &m_starsoulMace};

        s_globalBraveryMod.connect(this, &Decimators::grimHarvestors, &m_connection);
    }

    Decimators::~Decimators() {
        m_connection.disconnect();
    }

    bool Decimators::configure(int numModels, int numStarsoulMaces) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }
        const int maxStarsoulMaces = (numModels / 5) * 2;
        if (numStarsoulMaces > maxStarsoulMaces) {
            // Invalid weapon configuration.
            return false;
        }

        // Add the Prime
        auto primeModel = new Model(BASESIZE, wounds());
        primeModel->addMeleeWeapon(&m_thunderaxePrime);
        addModel(primeModel);

        for (auto i = 0; i < numStarsoulMaces; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_starsoulMace);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_thunderaxe);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds Decimators::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Starsoul Mace
        if (weapon->name() == m_starsoulMace.name()) {
            int mortalWounds = 0;
            int roll = Dice::rollD6();
            if (roll >= 6) {
                mortalWounds = Dice::rollD3() + 1;
            } else if (roll >= 2) {
                mortalWounds = Dice::rollD3();
            }
            return {0, mortalWounds};
        }
        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Unit *Decimators::Create(const ParameterList &parameters) {
        auto unit = new Decimators();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        int numStarsoulMaces = GetIntParam("Starsoul Maces", parameters, 0);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, NoStormhost);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels, numStarsoulMaces);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Decimators::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            IntegerParameter("Starsoul Maces", 2, 0, (MAX_UNIT_SIZE / 5) * 2, 1),
                            EnumParameter("Stormhost", NoStormhost, g_stormhost)
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
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
