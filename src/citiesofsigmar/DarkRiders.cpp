/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DarkRiders.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 65;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 360;

    bool DarkRiders::s_registered = false;

    Unit *DarkRiders::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);
        return new DarkRiders(city, numModels, standard, hornblower, ComputePoints(parameters));
    }

    std::string DarkRiders::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int DarkRiders::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void DarkRiders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };

            s_registered = UnitFactory::Register("Dark Riders", factoryMethod);
        }
    }

    DarkRiders::DarkRiders(City city, int numModels, bool standardBearer, bool hornblower, int points) :
            CitizenOfSigmar(city, "Dark Riders", 14, g_wounds, 6, 4, false, points),
            m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 3, 5, 4, 0, 1),
            m_spear(Weapon::Type::Melee, "Barbed Spear", 2, 1, 4, 4, 0, 1),
            m_bite(Weapon::Type::Melee, "Vicious Bite", 1, 2, 4, 5, 0, 1),
            m_crossbowHerald(Weapon::Type::Missile, "Repeater Crossbow", 16, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SHADOWBLADES, DARK_RIDERS};
        m_weapons = {&m_crossbow, &m_spear, &m_bite, &m_crossbowHerald};
        m_hasMount = true;
        m_bite.setMount(true);

        s_globalBraveryMod.connect(this, &DarkRiders::sowTerrorAndConfusion, &m_connection);

        // Add the Herald
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMissileWeapon(&m_crossbowHerald);
        bossModel->addMeleeWeapon(&m_spear);
        bossModel->addMeleeWeapon(&m_bite);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_crossbow);
            model->addMeleeWeapon(&m_spear);
            model->addMeleeWeapon(&m_bite);
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

    DarkRiders::~DarkRiders() {
        m_connection.disconnect();
    }

    int DarkRiders::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int DarkRiders::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    Wounds DarkRiders::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Murderous Charge
        if (m_charged && (weapon->name() == m_spear.name())) {
            return {2, 0};
        }
        return CitizenOfSigmar::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int DarkRiders::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int DarkRiders::sowTerrorAndConfusion(const Unit *target) {
        // Sow Terror and Confusion
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 12.0)) {
            return -1;
        }

        return 0;
    }

} //namespace CitiesOfSigmar