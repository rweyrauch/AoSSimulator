/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Zombies.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 20;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 115;
    static const int g_pointsMaxUnitSize = 2 * g_pointsPerBlock;

    bool Zombies::s_registered = false;

    Zombies::Zombies(CursedBloodline bloodline, int numModels, int points) :
            SoulblightBase(bloodline, "Deadwalker Zombies", 4, g_wounds, 10, NoSave, false, points),
            m_zombieBite(Weapon::Type::Melee, "Crude Weapons and Infectious Bites", 1, 1, 5, 5, 0, 1) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEADWALKERS, SUMMONABLE, DEADWALKER_ZOMBIES};
        m_weapons = {&m_zombieBite};
        m_battleFieldRole = Role::Battleline;

        // Dragged Down and Torn Apart
        m_pileInMove = 6.0;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_zombieBite);
            addModel(model);
        }
    }

    Zombies::~Zombies() {
    }

    Unit *Zombies::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new Zombies(bloodline, numModels, ComputePoints(parameters));
    }

    void Zombies::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Deadwalker Zombies", factoryMethod);
        }
    }

    int Zombies::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Zombies::onEndCombat(PlayerId player) {
        SoulblightBase::onEndCombat(player);

        // The Newly Dead
        if (m_currentRecord.m_enemyModelsSlain > 0) {
            Dice::RollResult rolls;
            Dice::RollD6(m_currentRecord.m_enemyModelsSlain, rolls);
            restoreModels(rolls.rollsGE(2));
        }
    }

    Wounds
    Zombies::weaponDamage(const Model *attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // The Newly Dead
        if (hitRoll == 6) {
            return {0, 1, Wounds::Source::Weapon_Melee, weapon};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} //namespace Soulblight
