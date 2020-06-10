/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Executioners.h>
#include <UnitFactory.h>

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 330;

    bool Executioners::s_registered = false;

    Unit *Executioners::Create(const ParameterList &parameters) {
        auto unit = new Executioners();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool drummer = GetBoolParam("Drummer", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, Hammerhal);
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
            CitizenOfSigmar("Executioners", 6, WOUNDS, 7, 4, false),
            m_draich(Weapon::Type::Melee, "Executioner's Draich", 1, 2, 3, 3, 0, 1),
            m_draichMaster(Weapon::Type::Melee, "Executioner's Draich", 1, 3, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, EXECUTIONERS};
        m_weapons = {&m_draich, &m_draichMaster};
    }

    bool Executioners::configure(int numModels, bool standardBearer, bool drummer) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_drummer = drummer;

        // Add the Master
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMeleeWeapon(&m_draichMaster);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_draich);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Executioners::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_drummer) mod++;
        return mod;
    }

    int Executioners::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_drummer) mod++;
        return mod;
    }

    int Executioners::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar