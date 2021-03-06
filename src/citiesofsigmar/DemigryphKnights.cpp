/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DemigryphKnights.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 65;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool DemigryphKnights::s_registered = false;

    Unit *DemigryphKnights::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Lance);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);
        return new DemigryphKnights(city, numModels, standard, hornblower, weapons, ComputePoints(parameters));
    }

    std::string DemigryphKnights::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Halberd) {
                return "Halberd";
            } else if (parameter.intValue == Lance) {
                return "Lance";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int DemigryphKnights::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Halberd") {
            return Halberd;
        } else if (enumString == "Lance") {
            return Lance;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void DemigryphKnights::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {DemigryphKnights::Halberd, DemigryphKnights::Lance};
            static FactoryMethod factoryMethod = {
                    DemigryphKnights::Create,
                    DemigryphKnights::ValueToString,
                    DemigryphKnights::EnumStringToInt,
                    DemigryphKnights::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", DemigryphKnights::Lance, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Demigryph Knights", factoryMethod);
        }
    }

    DemigryphKnights::DemigryphKnights(City city, int numModels, bool standardBearer, bool hornblower, WeaponOption weapons, int points) :
            CitizenOfSigmar(city, "Demigryph Knights", 10, g_wounds, 6, 3, false, points),
            m_halberd(Weapon::Type::Melee, "Demigryph Knight's Halberd", 2, 3, 3, 3, -1, 1),
            m_lance(Weapon::Type::Melee, "Demigryph Knight's Lance", 2, 3, 3, 4, 0, 1),
            m_halberdPreceptor(Weapon::Type::Melee, "Demigryph Knight's Halberd", 2, 4, 3, 3, -1, 1),
            m_lancePreceptor(Weapon::Type::Melee, "Demigryph Knight's Lance", 2, 4, 3, 4, 0, 1),
            m_beakAndTalons(Weapon::Type::Melee, "Beak and Talons", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, DEMIGRYPH_KNIGHTS};
        m_weapons = {&m_halberd, &m_lance, &m_halberdPreceptor, &m_lancePreceptor, &m_beakAndTalons};
        m_hasMount = true;
        m_beakAndTalons.setMount(true);

        // Add the Preceptor
        auto bossModel = new Model(g_basesize, wounds());
        if (weapons == Halberd) {
            bossModel->addMeleeWeapon(&m_halberdPreceptor);
        } else if (weapons == Lance) {
            bossModel->addMeleeWeapon(&m_lancePreceptor);
        }
        bossModel->addMeleeWeapon(&m_beakAndTalons);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Halberd) {
                model->addMeleeWeapon(&m_halberd);
            } else if (weapons == Lance) {
                model->addMeleeWeapon(&m_lance);
            }
            model->addMeleeWeapon(&m_beakAndTalons);
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

    int DemigryphKnights::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int DemigryphKnights::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int DemigryphKnights::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    Wounds DemigryphKnights::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Charging Lance
        if (m_charged && (weapon->name() == m_lance.name())) {
            return {2, 0};
        }

        // Savage Ferocity
        if ((woundRoll == 6) && (weapon->name() == m_beakAndTalons.name())) {
            return {1, 0};
        }
        return CitizenOfSigmar::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int DemigryphKnights::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Charging Lance
        if (m_charged && (weapon->name() == m_lance.name())) {
            return -2;
        }
        return CitizenOfSigmar::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int DemigryphKnights::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar
