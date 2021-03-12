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
    static const int g_basesize = 60;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool DrakespawnKnights::s_registered = false;

    Unit *DrakespawnKnights::Create(const ParameterList &parameters) {
        auto unit = new DrakespawnKnights();

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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            {ParamType::Boolean, "Standard Bearer", Sim_True, Sim_False, Sim_False, 0},
                            {ParamType::Boolean, "Hornblower", Sim_True, Sim_False, Sim_False, 0},
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Drakespawn Knights", factoryMethod);
        }
    }

    DrakespawnKnights::DrakespawnKnights() :
            CitizenOfSigmar("Drakespawn Knights", 10, g_wounds, 7, 3, false),
            m_lance(Weapon::Type::Melee, "Barbed Lance", 2, 1, 3, 4, -1, 1),
            m_lanceDreadKnight(Weapon::Type::Melee, "Barbed Lance", 2, 2, 3, 4, -1, 1),
            m_jaws(Weapon::Type::Melee, "Ferocious Jaws", 1, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, DRAKESPAWN_KNIGHTS};
        m_weapons = {&m_lance, &m_lanceDreadKnight, &m_jaws};
        m_hasMount = true;
        m_jaws.setMount(true);
    }

    bool DrakespawnKnights::configure(int numModels, bool standardBearer, bool hornblower) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Dread Knight
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_lanceDreadKnight);
        bossModel->addMeleeWeapon(&m_jaws);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_lance);
            model->addMeleeWeapon(&m_jaws);
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

    int DrakespawnKnights::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int DrakespawnKnights::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int DrakespawnKnights::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    Wounds DrakespawnKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lance Charge
        if (m_charged && (weapon->name() == m_lance.name())) {
            return {2, 0};
        }

        return CitizenOfSigmar::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int DrakespawnKnights::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lance Charge
        if (m_charged && (weapon->name() == m_lance.name())) {
            return -2;
        }
        return CitizenOfSigmar::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int DrakespawnKnights::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar