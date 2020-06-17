/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/FreeguildHandgunners.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    bool FreeguildHandgunners::s_registered = false;

    Unit *FreeguildHandgunners::Create(const ParameterList &parameters) {
        auto unit = new FreeguildHandgunners();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);
        auto marksmanWeapon = (WeaponOption) GetEnumParam("Marksman Weapon", parameters, Handgun);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, hornblower, marksmanWeapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FreeguildHandgunners::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Marksman Weapon") {
            if (parameter.intValue == Handgun) {
                return "Handgun";
            } else if (parameter.intValue == LongRifle) {
                return "Long Rifle";
            } else if (parameter.intValue == RepeaterHandgun) {
                return "Repeater Handgun";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildHandgunners::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Handgun") {
            return Handgun;
        } else if (enumString == "Long Rifle") {
            return LongRifle;
        } else if (enumString == "Repeater Handgun") {
            return RepeaterHandgun;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildHandgunners::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Handgun, LongRifle, RepeaterHandgun};
            static FactoryMethod factoryMethod = {
                    FreeguildHandgunners::Create,
                    FreeguildHandgunners::ValueToString,
                    FreeguildHandgunners::EnumStringToInt,
                    FreeguildHandgunners::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter<3>("Marksman Weapon", Handgun, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Piper"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Freeguild Handgunners", factoryMethod);
        }
    }

    FreeguildHandgunners::FreeguildHandgunners() :
            CitizenOfSigmar("Freeguild Handgunners", 5, WOUNDS, 5, 6, false),
            m_freeguildHandgun(Weapon::Type::Missile, "Freeguild Handgun", 16, 1, 4, 3, -1, 1),
            m_dagger(Weapon::Type::Melee, "Dagger", 1, 1, 5, 5, 0, 1),
            m_longRifle(Weapon::Type::Missile, "Long Rifle", 30, 1, 4, 3, -1, 2),
            m_repeaterHandgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
            m_handgunMarksman(Weapon::Type::Missile, "Freeguild Handgun", 16, 1, 2, 3, -1, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_HANDGUNNERS};
        m_weapons = {&m_freeguildHandgun, &m_dagger, &m_longRifle, &m_repeaterHandgun, &m_handgunMarksman};
    }

    bool FreeguildHandgunners::configure(int numModels, bool standardBearer, bool piper, WeaponOption marksmanWeapon) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_piper = piper;

        // Add the Marksman
        auto bossModel = new Model(BASESIZE, wounds());
        if (marksmanWeapon == Handgun) {
            bossModel->addMissileWeapon(&m_handgunMarksman);
        } else if (marksmanWeapon == LongRifle) {
            bossModel->addMissileWeapon(&m_longRifle);
        } else if (marksmanWeapon == RepeaterHandgun) {
            bossModel->addMeleeWeapon(&m_repeaterHandgun);
        }
        bossModel->addMeleeWeapon(&m_dagger);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_freeguildHandgun);
            model->addMeleeWeapon(&m_dagger);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int FreeguildHandgunners::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_piper) mod++;
        return mod;
    }

    int FreeguildHandgunners::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_piper) mod++;
        return mod;
    }

    int FreeguildHandgunners::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    int FreeguildHandgunners::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Steady Aim
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        if ((remainingModels() >= 10) && !m_moved && units.empty()) mod++;

        return mod;
    }

    int FreeguildHandgunners::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar