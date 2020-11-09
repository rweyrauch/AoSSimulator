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
        auto unit = new WildRiders();

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

    WildRiders::WildRiders() :
            CitizenOfSigmar("Wild Riders", 12, g_wounds, 8, 5, false),
            m_spear(Weapon::Type::Melee, "Hunting Spear", 2, 2, 3, 4, -1, 1),
            m_hooves(Weapon::Type::Melee, "Antlers and Hooves", 1, 2, 4, 4, 0, 1),
            m_spearHunter(Weapon::Type::Melee, "Hunting Spear", 2, 3, 3, 4, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, WILD_RIDERS};
        m_weapons = {&m_spear, &m_hooves, &m_spearHunter};
        m_hasMount = true;
    }

    bool WildRiders::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        // Add the Hunter
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_spearHunter);
        bossModel->addMeleeWeapon(&m_hooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spear);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int WildRiders::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int WildRiders::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int WildRiders::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    Wounds WildRiders::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Unbound Fury
        if (m_charged && (weapon->name() == m_spear.name())) {
            return {2, 0};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int WildRiders::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Unbound Fury
        if (m_charged && (weapon->name() == m_spear.name())) {
            return -2;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int WildRiders::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar