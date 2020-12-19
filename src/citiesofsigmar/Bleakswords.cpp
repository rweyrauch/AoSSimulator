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
        auto unit = new Bleakswords();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    Bleakswords::Bleakswords() :
            CitizenOfSigmar("Bleakswords", 6, g_wounds, 6, 4, false),
            m_sword(Weapon::Type::Melee, "Darkling Sword", 1, 1, 4, 4, 0, 1),
            m_swordLordling(Weapon::Type::Melee, "Darkling Sword", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, BLEAKSWORDS};
        m_weapons = {&m_sword, &m_swordLordling};
        m_battleFieldRole = Battleline;
    }

    bool Bleakswords::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Lordling
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_swordLordling);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_sword);
            if (standardBearer) {
                model->setName("Standard Bearer");
                standardBearer = false;
            }
            else if (hornblower) {
                model->setName("Hornblower");
                hornblower = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Bleakswords::runModifier() const {
        auto mod = Unit::runModifier();
        if (isNamedModelAlive("Hornblower")) mod++;
        return mod;
    }

    int Bleakswords::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (isNamedModelAlive("Hornblower")) mod++;
        return mod;
    }

    int Bleakswords::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (isNamedModelAlive("Standard Bearer")) mod++;
        return mod;
    }

    int Bleakswords::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Quicksilver Strike
        if (unmodifiedHitRoll == 6) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int Bleakswords::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Ranks of Cold Steel
        if (remainingModels() >= 10) mod++;
        return mod;
    }

    int Bleakswords::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace CitiesOfSigmar