/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Bleakswords.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 320;

    bool Bleakswords::s_registered = false;

    Unit *Bleakswords::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);
        return new Bleakswords(city, numModels, standard, hornblower, ComputePoints(parameters));
    }

    std::string Bleakswords::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Bleakswords::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Bleakswords::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Bleakswords::Create,
                    Bleakswords::ValueToString,
                    Bleakswords::EnumStringToInt,
                    Bleakswords::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Bleakswords", factoryMethod);
        }
    }

    Bleakswords::Bleakswords(City city, int numModels, bool standardBearer, bool hornblower, int points) :
            CitizenOfSigmar(city, "Bleakswords", 6, g_wounds, 6, 4, false, points),
            m_sword(Weapon::Type::Melee, "Darkling Sword", 1, 1, 4, 4, 0, 1),
            m_swordLordling(Weapon::Type::Melee, "Darkling Sword", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, BLEAKSWORDS};
        m_weapons = {&m_sword, &m_swordLordling};
        m_battleFieldRole = Role::Battleline;

        // Add the Lordling
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_swordLordling);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_sword);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }
    }

    int Bleakswords::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int Bleakswords::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int Bleakswords::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int Bleakswords::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Quicksilver Strike
        if (unmodifiedHitRoll == 6) {
            return 2;
        }
        return CitizenOfSigmar::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int Bleakswords::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);
        // Ranks of Cold Steel
        if (remainingModels() >= 10) mod++;
        return mod;
    }

    int Bleakswords::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace CitiesOfSigmar