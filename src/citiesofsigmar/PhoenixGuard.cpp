/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/PhoenixGuard.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = 420;

    bool PhoenixGuard::s_registered = false;

    Unit *PhoenixGuard::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool drummer = GetBoolParam("Drummer", parameters, true);
        return new PhoenixGuard(city, numModels, standard, drummer, ComputePoints(parameters));
    }

    std::string PhoenixGuard::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int PhoenixGuard::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void PhoenixGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    PhoenixGuard::Create,
                    PhoenixGuard::ValueToString,
                    PhoenixGuard::EnumStringToInt,
                    PhoenixGuard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Drummer"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Phoenix Guard", factoryMethod);
        }
    }

    PhoenixGuard::PhoenixGuard(City city, int numModels, bool standardBearer, bool drummer, int points) :
            CitizenOfSigmar(city, "Phoenix Guard", 6, g_wounds, 7, 4, false, points),
            m_halberd(Weapon::Type::Melee, "Phoenix Halberd", 2, 2, 3, 3, -1, 1),
            m_halberdKeeper(Weapon::Type::Melee, "Phoenix Halberd", 2, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, PHOENIX_GUARD};
        m_weapons = {&m_halberd, &m_halberdKeeper};

        // Add the Keeper
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_halberdKeeper);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_halberd);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (drummer) {
                model->setName(Model::Drummer);
                drummer = false;
            }
            addModel(model);
        }
    }

    int PhoenixGuard::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Drummer)) mod++;
        return mod;
    }

    int PhoenixGuard::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Drummer)) mod++;
        return mod;
    }

    int PhoenixGuard::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    Wounds PhoenixGuard::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Witness to Destiny
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(wounds.normal, woundSaves);
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(4);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(4);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int PhoenixGuard::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    bool PhoenixGuard::battleshockRequired() const {
        // Emboldened
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
        for (auto unit : units) {
            if (unit->hasKeyword(PHOENIX_TEMPLE) && unit->hasKeyword(HERO)) return false;
        }
        return CitizenOfSigmar::battleshockRequired();
    }

} // namespace CitiesOfSigmar