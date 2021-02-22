/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Dreadspears.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 320;

    bool Dreadspears::s_registered = false;

    Unit *Dreadspears::Create(const ParameterList &parameters) {
        auto unit = new Dreadspears();

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

    std::string Dreadspears::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Dreadspears::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Dreadspears::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Dreadspears::Create,
                    Dreadspears::ValueToString,
                    Dreadspears::EnumStringToInt,
                    Dreadspears::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            {ParamType::Boolean, "Standard Bearer", Sim_True, Sim_False, Sim_False, 0},
                            {ParamType::Boolean, "Hornblower", Sim_True, Sim_False, Sim_False, 0},
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Dreadspears", factoryMethod);
        }
    }

    Dreadspears::Dreadspears() :
            CitizenOfSigmar("Dreadspears", 6, g_wounds, 6, 4, false),
            m_spear(Weapon::Type::Melee, "Darkling Spear", 2, 1, 4, 4, 0, 1),
            m_spearLordling(Weapon::Type::Melee, "Darkling Spear", 2, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, DREADSPEARS};
        m_weapons = {&m_spear, &m_spearLordling};
        m_battleFieldRole = Battleline;
    }

    bool Dreadspears::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Lordling
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_spearLordling);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spear);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            }
            else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Dreadspears::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int Dreadspears::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int Dreadspears::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int Dreadspears::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);
        // Ranks of Cold Steel
        if (remainingModels() >= 10) mod++;
        return mod;
    }

    int Dreadspears::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Coven Guard
        if (hitRoll == 6) {
            return -1;
        }
        return CitizenOfSigmar::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int Dreadspears::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar