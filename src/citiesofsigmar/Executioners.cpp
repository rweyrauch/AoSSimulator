/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Executioners.h>
#include <UnitFactory.h>
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 300;

    bool Executioners::s_registered = false;

    Unit *Executioners::Create(const ParameterList &parameters) {
        auto unit = new Executioners();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool drummer = GetBoolParam("Drummer", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, drummer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Executioners::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Executioners::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Executioners::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Executioners::Create,
                    Executioners::ValueToString,
                    Executioners::EnumStringToInt,
                    Executioners::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Drummer"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Executioners", factoryMethod);
        }
    }

    Executioners::Executioners() :
            CitizenOfSigmar("Executioners", 6, g_wounds, 7, 4, false),
            m_draich(Weapon::Type::Melee, "Executioner's Draich", 1, 2, 3, 3, 0, 1),
            m_draichMaster(Weapon::Type::Melee, "Executioner's Draich", 1, 3, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, EXECUTIONERS};
        m_weapons = {&m_draich, &m_draichMaster};
    }

    bool Executioners::configure(int numModels, bool standardBearer, bool drummer) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Master
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_draichMaster);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_draich);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            }
            else if (drummer) {
                model->setName(Model::Drummer);
                drummer = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Executioners::runModifier() const {
        auto mod = Unit::runModifier();
        if (isNamedModelAlive(Model::Drummer)) mod++;
        return mod;
    }

    int Executioners::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (isNamedModelAlive(Model::Drummer)) mod++;
        return mod;
    }

    int Executioners::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    Wounds Executioners::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Severing Strike
        if (hitRoll == 6) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Executioners::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar