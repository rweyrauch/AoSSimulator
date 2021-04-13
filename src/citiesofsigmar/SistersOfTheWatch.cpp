/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/SistersOfTheWatch.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 60;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = 480;

    bool SistersOfTheWatch::s_registered = false;

    Unit *SistersOfTheWatch::Create(const ParameterList &parameters) {
        auto unit = new SistersOfTheWatch();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string SistersOfTheWatch::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int SistersOfTheWatch::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void SistersOfTheWatch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SistersOfTheWatch::Create,
                    SistersOfTheWatch::ValueToString,
                    SistersOfTheWatch::EnumStringToInt,
                    SistersOfTheWatch::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Sisters of the Watch", factoryMethod);
        }
    }

    SistersOfTheWatch::SistersOfTheWatch() :
            CitizenOfSigmar("Sisters of the Watch", 6, g_wounds, 7, 5, false),
            m_bow(Weapon::Type::Missile, "Watch Bow", 18, 1, 3, 3, 0, 1),
            m_sword(Weapon::Type::Melee, "Ithilmar Sword", 1, 1, 4, 4, 0, 1),
            m_bowHighSister(Weapon::Type::Missile, "Watch Bow", 18, 2, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, SISTERS_OF_THE_WATCH};
        m_weapons = {&m_bow, &m_sword, &m_bowHighSister};
    }

    bool SistersOfTheWatch::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the High Sister
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMissileWeapon(&m_bowHighSister);
        bossModel->addMeleeWeapon(&m_sword);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_bow);
            model->addMeleeWeapon(&m_sword);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int SistersOfTheWatch::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extras = CitizenOfSigmar::extraAttacks(attackingModel, weapon, target);

        if (weapon->name() == m_bow.name()) {
            // Quicksilver Shot
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            if (units.empty()) {
                extras++;
            }
        }
        return extras;
    }

    Wounds SistersOfTheWatch::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Eldritch Arrows
        if ((woundRoll == 6) && (weapon->name() == m_bow.name())) {
            return {weapon->damage(), 1};
        }
        return CitizenOfSigmar::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int SistersOfTheWatch::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar