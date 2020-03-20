/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/BlackArkCorsairs.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool BlackArkCorsairs::s_registered = false;

    Unit *BlackArkCorsairs::Create(const ParameterList &parameters) {
        auto unit = new BlackArkCorsairs();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, WickedCutlass);

        auto city = (City) GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, hornblower, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string BlackArkCorsairs::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == RepeaterHandbow) {
                return "Repeater Handbow";
            } else if (parameter.intValue == WickedCutlass) {
                return "Wicked Cutlass";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int BlackArkCorsairs::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Repeater Handbow") {
            return RepeaterHandbow;
        } else if (enumString == "Wicked Cutlass") {
            return WickedCutlass;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void BlackArkCorsairs::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Weapons", WickedCutlass, RepeaterHandbow, WickedCutlass, 1},
                            {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal,
                             CitizenOfSigmar::TempestsEye, 1},
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Black Ark Corsairs", factoryMethod);
        }
    }

    BlackArkCorsairs::BlackArkCorsairs() :
            CitizenOfSigmar("Black Ark Corsairs", 6, WOUNDS, 6, 5, false),
            m_handbow(Weapon::Type::Missile, "Repeater Handbow", 9, 2, 5, 4, 0, 1),
            m_cutlass(Weapon::Type::Melee, "Wicked Cutlass", 1, 1, 4, 4, 0, 1),
            m_blade(Weapon::Type::Melee, "Vicious Blade", 1, 1, 4, 5, 0, 1),
            m_handbowReaver(Weapon::Type::Missile, "Repeater Handbow", 9, 2, 4, 4, 0, 1),
            m_cutlassReaver(Weapon::Type::Melee, "Wicked Cutlass", 1, 1, 3, 4, 0, 1),
            m_bladeReaver(Weapon::Type::Melee, "Vicious Blade", 1, 1, 3, 5, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, BLACK_ARK_CORSAIRS};
        m_weapons = {&m_handbow, &m_cutlass, &m_blade, &m_handbowReaver, &m_cutlassReaver, &m_bladeReaver};
    }

    bool BlackArkCorsairs::configure(int numModels, bool standardBearer, bool hornblower, WeaponOption weapons) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        // Add the Reaver
        auto bossModel = new Model(BASESIZE, wounds());
        if (weapons == RepeaterHandbow) {
            bossModel->addMissileWeapon(&m_handbowReaver);
        } else if (weapons == WickedCutlass) {
            bossModel->addMeleeWeapon(&m_cutlassReaver);
        }
        bossModel->addMeleeWeapon(&m_bladeReaver);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == RepeaterHandbow) {
                model->addMissileWeapon(&m_handbow);
            } else if (weapons == WickedCutlass) {
                model->addMeleeWeapon(&m_cutlass);
            }
            model->addMeleeWeapon(&m_blade);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int BlackArkCorsairs::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int BlackArkCorsairs::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int BlackArkCorsairs::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    int BlackArkCorsairs::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Flashing Steel
        if (remainingModels() >= 15) mod++;

        return mod;
    }

    int BlackArkCorsairs::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

}//namespace CitiesOfSigmar