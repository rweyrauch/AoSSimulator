/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/BlackGuard.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool BlackGuard::s_registered = false;

    Unit *BlackGuard::Create(const ParameterList &parameters) {
        auto unit = new BlackGuard();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool drummer = GetBoolParam("Drummer", parameters, true);

        bool ok = unit->configure(numModels, standard, drummer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string BlackGuard::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int BlackGuard::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void BlackGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Drummer"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Black Guard", factoryMethod);
        }
    }

    BlackGuard::BlackGuard() :
            CitizenOfSigmar("Black Guard", 6, g_wounds, 8, 4, false),
            m_halberd(Weapon::Type::Melee, "Ebon Halberd", 2, 2, 3, 3, -1, 1),
            m_halberdCaptain(Weapon::Type::Melee, "Ebon Halberd", 2, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, BLACK_GUARD};
        m_weapons = {&m_halberd, &m_halberdCaptain};
    }

    bool BlackGuard::configure(int numModels, bool standardBearer, bool drummer) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Captain
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_halberdCaptain);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_halberd);
            if (standardBearer) {
                model->setName("Standard Bearer");
                standardBearer = false;
            }
            else if (drummer) {
                model->setName("Drummer");
                drummer = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int BlackGuard::runModifier() const {
        auto mod = Unit::runModifier();
        if (isNamedModelAlive("Drummer")) mod++;
        return mod;
    }

    int BlackGuard::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (isNamedModelAlive("Drummer")) mod++;
        return mod;
    }

    int BlackGuard::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (isNamedModelAlive("Standard Bearer")) mod++;
        return mod;
    }

    int BlackGuard::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Elite Bodyguard
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 18.0);
        if (unit && unit->hasKeyword(DARKLING_COVENS)) mod++;
        return mod;
    }

    int BlackGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace CitiesOfSigmar