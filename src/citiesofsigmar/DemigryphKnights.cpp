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
    static const int BASESIZE = 65;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    bool DemigryphKnights::s_registered = false;

    Unit *DemigryphKnights::Create(const ParameterList &parameters) {
        auto unit = new DemigryphKnights();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);

        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Lance);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, hornblower, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
            static const std::array<int, 2> weapons = { DemigryphKnights::Halberd, DemigryphKnights::Lance};
            static FactoryMethod factoryMethod = {
                    DemigryphKnights::Create,
                    DemigryphKnights::ValueToString,
                    DemigryphKnights::EnumStringToInt,
                    DemigryphKnights::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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

    DemigryphKnights::DemigryphKnights() :
            CitizenOfSigmar("Demigryph Knights", 10, WOUNDS, 6, 3, false),
            m_halberd(Weapon::Type::Melee, "Demigryph Knight's Halberd", 2, 3, 3, 3, -1, 1),
            m_lance(Weapon::Type::Melee, "Demigryph Knight's Lance", 2, 3, 3, 4, 0, 1),
            m_halberdPreceptor(Weapon::Type::Melee, "Demigryph Knight's Halberd", 2, 4, 3, 3, -1, 1),
            m_lancePreceptor(Weapon::Type::Melee, "Demigryph Knight's Lance", 2, 4, 3, 4, 0, 1),
            m_beakAndTalons(Weapon::Type::Melee, "Beak and Talons", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, DEMIGRYPH_KNIGHTS};
        m_weapons = {&m_halberd, &m_lance, &m_halberdPreceptor, &m_lancePreceptor, &m_beakAndTalons};
    }

    bool DemigryphKnights::configure(int numModels, bool standardBearer, bool hornblower, WeaponOption weapons) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        // Add the Preceptor
        auto bossModel = new Model(BASESIZE, wounds());
        if (weapons == Halberd) {
            bossModel->addMeleeWeapon(&m_halberdPreceptor);
        } else if (weapons == Lance) {
            bossModel->addMeleeWeapon(&m_lancePreceptor);
        }
        bossModel->addMeleeWeapon(&m_beakAndTalons);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == Halberd) {
                model->addMeleeWeapon(&m_halberd);
            } else if (weapons == Lance) {
                model->addMeleeWeapon(&m_lance);
            }
            model->addMeleeWeapon(&m_beakAndTalons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int DemigryphKnights::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int DemigryphKnights::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int DemigryphKnights::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    Wounds DemigryphKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Charging Lance
        if (m_charged && (weapon->name() == m_lance.name())) {
            return {2, 0};
        }

        // Savage Ferocity
        if ((woundRoll == 6) && (weapon->name() == m_beakAndTalons.name())) {
            return {1, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int DemigryphKnights::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Charging Lance
        if (m_charged && (weapon->name() == m_lance.name())) {
            return -2;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int DemigryphKnights::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar
