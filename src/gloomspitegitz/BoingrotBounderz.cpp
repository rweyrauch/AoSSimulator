/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/BoingrotBounderz.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 300;

    bool BoingrotBounderz::s_registered = false;

    BoingrotBounderz::BoingrotBounderz(int numModels, int points) :
            GloomspiteGitzBase("Boingrot Bounderz", RAND_2D6, g_wounds, 5, 4, true, points),
            m_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1),
            m_pokinLance(Weapon::Type::Melee, "Pokin' Lance", 2, 2, 4, 4, -1, 1),
            m_pokinLanceBoss(Weapon::Type::Melee, "Pokin' Lance", 2, 2, 3, 4, -1, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, BOINGROT_BOUNDERZ};
        m_weapons = {&m_fangFilledGob, &m_pokinLance, &m_pokinLanceBoss};
        m_hasMount = true;
        m_fangFilledGob.setMount(true);

        // Add the boss
        auto boss = new Model(g_basesize, wounds());
        boss->addMeleeWeapon(&m_pokinLanceBoss);
        boss->addMeleeWeapon(&m_fangFilledGob);
        addModel(boss);

        // and the rest
        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_pokinLance);
            model->addMeleeWeapon(&m_fangFilledGob);
            addModel(model);
        }
    }

    Unit *BoingrotBounderz::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new BoingrotBounderz(numModels, ComputePoints(parameters));
    }

    void BoingrotBounderz::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BoingrotBounderz::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    BoingrotBounderz::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize)
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
                Dice::RollD6(numEnemyModels, rolls);
                int numMortalWounds = rolls.rollsGE(4);

                m_meleeTarget->applyDamage({0, numMortalWounds, Wounds::Source::Ability, nullptr}, this);
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

    int BoingrotBounderz::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace GloomspiteGitz
