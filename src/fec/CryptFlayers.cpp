/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptFlayers.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 170;
    static const int g_pointsMaxUnitSize = 170 * 4;

    bool CryptFlayers::s_registered = false;

    CryptFlayers::CryptFlayers(GrandCourt court, Delusion delusion, int numModels, int points) :
            FleshEaterCourts(court, delusion, "Crypt Flayers", 12, g_wounds, 10, 5, true, points),
            m_deathScream(Weapon::Type::Missile, "Death Scream", 10, 1, 0, 0, 0, 0),
            m_piercingTalons(Weapon::Type::Melee, "Piercing Talons", 1, 4, 4, 3, -1, 1),
            m_piercingTalonsInfernal(Weapon::Type::Melee, "Piercing Talons", 1, 5, 4, 3, -1, 1) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, KNIGHTS, CRYPT_FLAYERS};
        m_weapons = {&m_deathScream, &m_piercingTalons, &m_piercingTalonsInfernal};

        auto infernal = new Model(g_basesize, wounds());
        infernal->addMissileWeapon(&m_deathScream);
        infernal->addMeleeWeapon(&m_piercingTalonsInfernal);
        addModel(infernal);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_deathScream);
            model->addMeleeWeapon(&m_piercingTalons);
            addModel(model);
        }
    }

    Unit *CryptFlayers::Create(const ParameterList &parameters) {
        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new CryptFlayers(court, delusion, numModels, ComputePoints(parameters));
    }

    void CryptFlayers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Crypt Flayers", factoryMethod);
        }
    }

    Wounds CryptFlayers::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Skewering Strike
        if ((hitRoll == 6) && weapon->name() == m_piercingTalons.name()) {
            // One mortal wound.
            return {0, 1};
        }

        // Death Scream
        if (weapon->name() == m_deathScream.name()) {
            int roll = Dice::Roll2D6();
            if (distanceTo(target) > 3.0)
                roll -= 2;
            if (roll > target->bravery()) {
                return {0, roll - target->bravery()};
            }
        }
        return FleshEaterCourts::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int CryptFlayers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }


} // namespace FleshEaterCourt
