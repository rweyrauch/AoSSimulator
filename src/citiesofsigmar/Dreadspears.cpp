/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Dreadspears.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool Dreadspears::s_registered = false;

    Unit *Dreadspears::Create(const ParameterList &parameters) {
        auto unit = new Dreadspears();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, Hammerhal);
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Dreadspears", factoryMethod);
        }
    }

    Dreadspears::Dreadspears() :
            CitizenOfSigmar("Dreadspears", 6, WOUNDS, 6, 4, false),
            m_spear(Weapon::Type::Melee, "Darkling Spear", 2, 1, 4, 4, 0, 1),
            m_spearLordling(Weapon::Type::Melee, "Darkling Spear", 2, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, DREADSPEARS};
        m_weapons = {&m_spear, &m_spearLordling};
    }

    bool Dreadspears::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        // Add the Lordling
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMeleeWeapon(&m_spearLordling);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_spear);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Dreadspears::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int Dreadspears::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int Dreadspears::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    int Dreadspears::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Ranks of Cold Steel
        if (remainingModels() >= 10) mod++;
        return mod;
    }

    int Dreadspears::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Coven Guard
        if (hitRoll == 6) {
            return -1;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int Dreadspears::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar