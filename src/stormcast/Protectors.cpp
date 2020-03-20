/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Protectors.h>
#include <UnitFactory.h>
#include <iostream>

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    bool Protectors::s_registered = false;

    Protectors::Protectors() :
            StormcastEternal("Protectors", 4, WOUNDS, 7, 4, false),
            m_glaive(Weapon::Type::Melee, "Stormstrike Glaive", 3, 3, 3, 3, -1, 1),
            m_glaivePrime(Weapon::Type::Melee, "Stormstrike Glaive", 3, 4, 3, 3, -1, 1),
            m_starsoulMace(Weapon::Type::Melee, "Starsoul Mace", 1, 1, 0, 0, 0, 0) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, PALADIN, PROTECTORS};
        m_weapons = {&m_glaive, &m_glaivePrime, &m_starsoulMace};
    }

    bool Protectors::configure(int numModels, int numStarsoulMaces) {
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
        primeModel->addMeleeWeapon(&m_glaivePrime);
        addModel(primeModel);

        for (auto i = 0; i < numStarsoulMaces; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_starsoulMace);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_glaive);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds Protectors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deathstrike
        if ((hitRoll == 6) && target->hasKeyword(MONSTER) && (weapon->name() == m_glaive.name())) {
            return {Dice::rollD6(), 0};
        }
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

    Unit *Protectors::Create(const ParameterList &parameters) {
        auto unit = new Protectors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        int numStarsoulMaces = GetIntParam("Starsoul Maces", parameters, 0);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, StormcastEternal::None);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels, numStarsoulMaces);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Protectors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Integer, "Starsoul Maces", 2, 0, (MAX_UNIT_SIZE / 5) * 2, 1},
                            {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None,
                             StormcastEternal::AstralTemplars, 1},
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

    int Protectors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace StormcastEternals
