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
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool Darkshards::s_registered = false;

    Unit *Darkshards::Create(const ParameterList &parameters) {
        auto unit = new Darkshards();

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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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
            CitizenOfSigmar("Darkshards", 6, g_wounds, 6, 5, false),
            m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 2, 4, 4, 0, 1),
            m_dagger(Weapon::Type::Melee, "Cruel Dagger", 1, 1, 5, 5, 0, 1),
            m_crossbowMaster(Weapon::Type::Missile, "Repeater Crossbow", 16, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, DARKSHARDS};
        m_weapons = {&m_crossbow, &m_dagger, &m_crossbowMaster};
        m_battleFieldRole = Battleline;
    }

    bool Darkshards::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Guardmaster
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMissileWeapon(&m_crossbowMaster);
        bossModel->addMeleeWeapon(&m_dagger);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_crossbow);
            model->addMeleeWeapon(&m_dagger);
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

    int Darkshards::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int Darkshards::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int Darkshards::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int Darkshards::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);
        // Storm of Iron-tipped Bolts
        if ((remainingModels() >= 10) && (weapon->name() == m_crossbow.name())) mod++;
        return mod;
    }

    int Darkshards::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace CitiesOfSigmar