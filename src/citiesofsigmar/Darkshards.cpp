/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Darkshards.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    bool Darkshards::s_registered = false;

    Unit *Darkshards::Create(const ParameterList &parameters) {
        auto unit = new Darkshards();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
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

    std::string Darkshards::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Darkshards::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Darkshards::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Darkshards::Create,
                    Darkshards::ValueToString,
                    Darkshards::EnumStringToInt,
                    Darkshards::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Darkshards", factoryMethod);
        }
    }

    Darkshards::Darkshards() :
            CitizenOfSigmar("Darkshards", 6, WOUNDS, 6, 5, false),
            m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 2, 4, 4, 0, 1),
            m_dagger(Weapon::Type::Melee, "Cruel Dagger", 1, 1, 5, 5, 0, 1),
            m_crossbowMaster(Weapon::Type::Missile, "Repeater Crossbow", 16, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, DARKSHARDS};
        m_weapons = {&m_crossbow, &m_dagger, &m_crossbowMaster};
    }

    bool Darkshards::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        // Add the Guardmaster
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMissileWeapon(&m_crossbowMaster);
        bossModel->addMeleeWeapon(&m_dagger);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_crossbow);
            model->addMeleeWeapon(&m_dagger);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Darkshards::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int Darkshards::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int Darkshards::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    int Darkshards::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Storm of Iron-tipped Bolts
        if ((remainingModels() >= 10) && (weapon->name() == m_crossbow.name())) mod++;
        return mod;
    }

    int Darkshards::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace CitiesOfSigmar