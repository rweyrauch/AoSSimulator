/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/WildRiders.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 60;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool WildRiders::s_registered = false;

    Unit *WildRiders::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);
        return new WildRiders(city, numModels, standard, hornblower, ComputePoints(parameters));
    }

    std::string WildRiders::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int WildRiders::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void WildRiders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WildRiders::Create,
                    WildRiders::ValueToString,
                    WildRiders::EnumStringToInt,
                    WildRiders::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Wild Riders", factoryMethod);
        }
    }

    WildRiders::WildRiders(City city, int numModels, bool standardBearer, bool hornblower, int points) :
            CitizenOfSigmar(city, "Wild Riders", 12, g_wounds, 8, 5, false, points),
            m_spear(Weapon::Type::Melee, "Hunting Spear", 2, 2, 3, 4, -1, 1),
            m_hooves(Weapon::Type::Melee, "Antlers and Hooves", 1, 2, 4, 4, 0, 1),
            m_spearHunter(Weapon::Type::Melee, "Hunting Spear", 2, 3, 3, 4, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, WILD_RIDERS};
        m_weapons = {&m_spear, &m_hooves, &m_spearHunter};
        m_hasMount = true;
        m_hooves.setMount(true);

        // Add the Hunter
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_spearHunter);
        bossModel->addMeleeWeapon(&m_hooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spear);
            model->addMeleeWeapon(&m_hooves);
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

    int WildRiders::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int WildRiders::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int WildRiders::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    Wounds WildRiders::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Unbound Fury
        if (m_charged && (weapon->name() == m_spear.name())) {
            return {2, 0};
        }

        return CitizenOfSigmar::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int WildRiders::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Unbound Fury
        if (m_charged && (weapon->name() == m_spear.name())) {
            return -2;
        }
        return CitizenOfSigmar::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int WildRiders::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar