/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Kroxigor.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 560;

    bool Kroxigor::s_registered = false;

    Kroxigor::Kroxigor(WayOfTheSeraphon way, Constellation constellation, int numModels, int numMoonhammers) :
            SeraphonBase("Kroxigor", 8, g_wounds, 7, 4, false) {
        m_keywords = {ORDER, SERAPHON, KROXIGOR};
        m_weapons = {&m_maul, &m_hammer, &m_jaws};

        setWayOfTheSeraphon(way, constellation);

        for (auto i = 0; i < numMoonhammers; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_hammer);
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }
        for (auto i = numMoonhammers; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_maul);
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Unit *Kroxigor::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numHammers = GetIntParam("Moon Hammers", parameters, g_minUnitSize / 3);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);

        return new Kroxigor(way, constellation, numModels, numHammers);
    }

    void Kroxigor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Moon Hammers", 1, 0, g_maxUnitSize / 3, 1),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };
            s_registered = UnitFactory::Register("Kroxigor", factoryMethod);
        }
    }


    int Kroxigor::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Kroxigor::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SeraphonBase::toHitModifier(weapon, target);

        // Battle Synergy
        auto skinks = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), SKINK, 6.0);
        if (skinks) mod++;

        return mod;
    }

    int Kroxigor::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SeraphonBase::extraAttacks(attackingModel, weapon, target);
        if ((weapon->name() == m_hammer.name())) {
            extra += getModelsWithin(attackingModel, target, 2.0);
        }
        return extra;
    }

    Wounds Kroxigor::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Jaws Like a Steel Trap
        if ((hitRoll == 6) && (weapon->name() == m_jaws.name())) {
            return {weapon->damage(), 1};
        }
        return SeraphonBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} //namespace Seraphon
