/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/BlackArkCorsairs.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool BlackArkCorsairs::s_registered = false;

    Unit *BlackArkCorsairs::Create(const ParameterList &parameters) {
        auto unit = new BlackArkCorsairs();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Wicked_Cutlass);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
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
            if (parameter.intValue == Repeater_Handbow) {
                return "Repeater Handbow";
            } else if (parameter.intValue == Wicked_Cutlass) {
                return "Wicked Cutlass";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int BlackArkCorsairs::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Repeater Handbow") {
            return Repeater_Handbow;
        } else if (enumString == "Wicked Cutlass") {
            return Wicked_Cutlass;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void BlackArkCorsairs::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Repeater_Handbow, Wicked_Cutlass};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Wicked_Cutlass, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Black Ark Corsairs", factoryMethod);
        }
    }

    BlackArkCorsairs::BlackArkCorsairs() :
            CitizenOfSigmar("Black Ark Corsairs", 6, g_wounds, 6, 5, false),
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
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Reaver
        auto bossModel = new Model(g_basesize, wounds());
        if (weapons == Repeater_Handbow) {
            bossModel->addMissileWeapon(&m_handbowReaver);
        } else if (weapons == Wicked_Cutlass) {
            bossModel->addMeleeWeapon(&m_cutlassReaver);
        }
        bossModel->addMeleeWeapon(&m_bladeReaver);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Repeater_Handbow) {
                model->addMissileWeapon(&m_handbow);
            } else if (weapons == Wicked_Cutlass) {
                model->addMeleeWeapon(&m_cutlass);
            }
            model->addMeleeWeapon(&m_blade);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int BlackArkCorsairs::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int BlackArkCorsairs::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int BlackArkCorsairs::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int BlackArkCorsairs::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Flashing Steel
        if (remainingModels() >= 15) mod++;

        return mod;
    }

    int BlackArkCorsairs::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

}//namespace CitiesOfSigmar