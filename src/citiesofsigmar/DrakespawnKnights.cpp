/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DrakespawnKnights.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 60;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 150 * 4;

    bool DrakespawnKnights::s_registered = false;

    Unit *DrakespawnKnights::Create(const ParameterList &parameters) {
        auto unit = new DrakespawnKnights();

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

    std::string DrakespawnKnights::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int DrakespawnKnights::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void DrakespawnKnights::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DrakespawnKnights::Create,
                    DrakespawnKnights::ValueToString,
                    DrakespawnKnights::EnumStringToInt,
                    DrakespawnKnights::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Drakespawn Knights", factoryMethod);
        }
    }

    DrakespawnKnights::DrakespawnKnights() :
            CitizenOfSigmar("Drakespawn Knights", 10, WOUNDS, 7, 3, false),
            m_lance(Weapon::Type::Melee, "Barbed Lance", 2, 1, 3, 4, -1, 1),
            m_lanceDreadKnight(Weapon::Type::Melee, "Barbed Lance", 2, 2, 3, 4, -1, 1),
            m_jaws(Weapon::Type::Melee, "Ferocious Jaws", 1, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, DRAKESPAWN_KNIGHTS};
        m_weapons = {&m_lance, &m_lanceDreadKnight, &m_jaws};
        m_hasMount = true;
    }

    bool DrakespawnKnights::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        // Add the Dread Knight
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMeleeWeapon(&m_lanceDreadKnight);
        bossModel->addMeleeWeapon(&m_jaws);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_lance);
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int DrakespawnKnights::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int DrakespawnKnights::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int DrakespawnKnights::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    Wounds DrakespawnKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lance Charge
        if (m_charged && (weapon->name() == m_lance.name())) {
            return {2, 0};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int DrakespawnKnights::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lance Charge
        if (m_charged && (weapon->name() == m_lance.name())) {
            return -2;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int DrakespawnKnights::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar