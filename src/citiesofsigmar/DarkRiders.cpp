/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DarkRiders.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 65;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool DarkRiders::s_registered = false;

    Unit *DarkRiders::Create(const ParameterList &parameters) {
        auto unit = new DarkRiders();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal,
                             CitizenOfSigmar::TempestsEye, 1},
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };

            s_registered = UnitFactory::Register("Dark Riders", factoryMethod);
        }
    }

    DarkRiders::DarkRiders() :
            CitizenOfSigmar("Dark Riders", 14, WOUNDS, 6, 4, false),
            m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 3, 5, 4, 0, 1),
            m_spear(Weapon::Type::Melee, "Barbed Spear", 2, 1, 4, 4, 0, 1),
            m_bite(Weapon::Type::Melee, "Vicious Bite", 1, 2, 4, 5, 0, 1),
            m_crossbowHerald(Weapon::Type::Missile, "Repeater Crossbow", 16, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SHADOWBLADES, DARK_RIDERS};
        m_weapons = {&m_crossbow, &m_spear, &m_bite, &m_crossbowHerald};

        s_globalBraveryMod.connect(this, &DarkRiders::sowTerrorAndConfusion, &m_connection);
    }

    DarkRiders::~DarkRiders() {
        m_connection.disconnect();
    }

    bool DarkRiders::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        // Add the Herald
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMissileWeapon(&m_crossbowHerald);
        bossModel->addMeleeWeapon(&m_spear);
        bossModel->addMeleeWeapon(&m_bite);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_crossbow);
            model->addMeleeWeapon(&m_spear);
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int DarkRiders::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int DarkRiders::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    Wounds DarkRiders::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Murderous Charge
        if (m_charged && (weapon->name() == m_spear.name())) {
            return {2, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int DarkRiders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
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