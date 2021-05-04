/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/EternalGuard.h>
#include <UnitFactory.h>
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = 330;

    bool EternalGuard::s_registered = false;

    EternalGuard::EternalGuard(City city, int numModels, bool standardBearer, bool hornblower, bool gladeShields, int points) :
            CitizenOfSigmar(city, "Eternal Guard", 6, g_wounds, 7, 4, false, points),
            m_spearStave(Weapon::Type::Melee, "Spear-stave", 2, 2, 3, 4, 0, 1),
            m_spearStaveWarden(Weapon::Type::Melee, "Spear-stave", 2, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, ETERNAL_GUARD};
        m_weapons = {&m_spearStave, &m_spearStaveWarden};
        m_battleFieldRole = Role::Battleline;

        m_gladeShields = gladeShields;

        auto lord = new Model(g_basesize, wounds());
        lord->addMeleeWeapon(&m_spearStaveWarden);
        addModel(lord);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spearStave);
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

    Unit *EternalGuard::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);
        bool shields = GetBoolParam("Glade Shields", parameters, false);
        return new EternalGuard(city, numModels, standardBearer, hornblower, shields, ComputePoints(parameters));
    }

    void EternalGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    EternalGuard::Create,
                    CitizenOfSigmar::ValueToString,
                    CitizenOfSigmar::EnumStringToInt,
                    EternalGuard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            BoolParameter("Glade Shields"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Eternal Guard", factoryMethod);
        }
    }

    int EternalGuard::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int EternalGuard::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int EternalGuard::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int EternalGuard::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);

        // Form Fortress of Boughs
        if (!m_moved) mod++;

        return mod;
    }

    int EternalGuard::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toWoundModifier(weapon, target);

        // Form Fortress of Boughs
        if (!m_moved) mod++;

        return mod;
    }

    int EternalGuard::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = CitizenOfSigmar::toSaveModifier(weapon, attacker);

        // Fortress of Boughs
        if (!m_moved) mod++;

        return mod;
    }

    int EternalGuard::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar
