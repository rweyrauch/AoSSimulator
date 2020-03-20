/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/BoingrotBounderz.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    bool BoingrotBounderz::s_registered = false;

    BoingrotBounderz::BoingrotBounderz() :
            GloomspiteGitzBase("Boingrot Bounderz", RAND_2D6, WOUNDS, 5, 4, true),
            m_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1),
            m_pokinLance(Weapon::Type::Melee, "Pokin' Lance", 2, 2, 4, 4, -1, 1),
            m_pokinLanceBoss(Weapon::Type::Melee, "Pokin' Lance", 2, 2, 3, 4, -1, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, BOINGROT_BOUNDERZ};
        m_weapons = {&m_fangFilledGob, &m_pokinLance, &m_pokinLanceBoss};
    }

    bool BoingrotBounderz::configure(int numModels) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        // Add the boss
        auto boss = new Model(BASESIZE, wounds());
        boss->addMeleeWeapon(&m_pokinLanceBoss);
        boss->addMeleeWeapon(&m_fangFilledGob);
        addModel(boss);

        // and the rest
        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_pokinLance);
            model->addMeleeWeapon(&m_fangFilledGob);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BoingrotBounderz::Create(const ParameterList &parameters) {
        auto unit = new BoingrotBounderz();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BoingrotBounderz::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BoingrotBounderz::Create,
                    nullptr,
                    nullptr,
                    BoingrotBounderz::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE}
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Boingrot Bounderz", factoryMethod);
        }
    }

    void BoingrotBounderz::onCharged() {
        if (m_meleeTarget) {
            // Boing! Smash!
            auto dist = distanceTo(m_meleeTarget);
            if (dist <= 1) {
                int numEnemyModels = m_meleeTarget->remainingModels();

                Dice::RollResult rolls;
                Dice::rollD6(numEnemyModels, rolls);
                int numMortalWounds = rolls.rollsGE(4);

                m_meleeTarget->applyDamage({0, numMortalWounds});
            }
        }
        GloomspiteGitzBase::onCharged();
    }

    int BoingrotBounderz::toWoundModifier(const Weapon *weapon, const Unit *unit) const {
        // Lances of the Bounderz
        int modifier = GloomspiteGitzBase::toWoundModifier(weapon, unit);
        if (m_charged && weapon->name() == m_pokinLance.name()) {
            modifier += 1;
        }

        return modifier;
    }

    int BoingrotBounderz::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace GloomspiteGitz
