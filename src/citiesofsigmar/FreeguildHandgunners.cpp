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
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 300;

    bool FreeguildHandgunners::s_registered = false;

    Unit *FreeguildHandgunners::Create(const ParameterList &parameters) {
        auto unit = new FreeguildHandgunners();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
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
            } else if (parameter.intValue == Long_Rifle) {
                return "Long Rifle";
            } else if (parameter.intValue == Repeater_Handgun) {
                return "Repeater Handgun";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildHandgunners::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Handgun") {
            return Handgun;
        } else if (enumString == "Long Rifle") {
            return Long_Rifle;
        } else if (enumString == "Repeater Handgun") {
            return Repeater_Handgun;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildHandgunners::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Handgun, Long_Rifle, Repeater_Handgun};
            static FactoryMethod factoryMethod = {
                    FreeguildHandgunners::Create,
                    FreeguildHandgunners::ValueToString,
                    FreeguildHandgunners::EnumStringToInt,
                    FreeguildHandgunners::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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
            CitizenOfSigmar("Freeguild Handgunners", 5, g_wounds, 5, 6, false),
            m_freeguildHandgun(Weapon::Type::Missile, "Freeguild Handgun", 16, 1, 4, 3, -1, 1),
            m_dagger(Weapon::Type::Melee, "Dagger", 1, 1, 5, 5, 0, 1),
            m_longRifle(Weapon::Type::Missile, "Long Rifle", 30, 1, 4, 3, -1, 2),
            m_repeaterHandgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
            m_handgunMarksman(Weapon::Type::Missile, "Freeguild Handgun", 16, 1, 2, 3, -1, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_HANDGUNNERS};
        m_weapons = {&m_freeguildHandgun, &m_dagger, &m_longRifle, &m_repeaterHandgun, &m_handgunMarksman};
        m_battleFieldRole = Battleline;
    }

    bool FreeguildHandgunners::configure(int numModels, bool standardBearer, bool piper, WeaponOption marksmanWeapon) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Marksman
        auto bossModel = new Model(g_basesize, wounds());
        if (marksmanWeapon == Handgun) {
            bossModel->addMissileWeapon(&m_handgunMarksman);
        } else if (marksmanWeapon == Long_Rifle) {
            bossModel->addMissileWeapon(&m_longRifle);
        } else if (marksmanWeapon == Repeater_Handgun) {
            bossModel->addMeleeWeapon(&m_repeaterHandgun);
        }
        bossModel->addMeleeWeapon(&m_dagger);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_freeguildHandgun);
            model->addMeleeWeapon(&m_dagger);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            }
            else if (piper) {
                model->setName(Model::Piper);
                piper = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int FreeguildHandgunners::runModifier() const {
        auto mod = Unit::runModifier();
        if (isNamedModelAlive(Model::Piper)) mod++;
        return mod;
    }

    int FreeguildHandgunners::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (isNamedModelAlive(Model::Piper)) mod++;
        return mod;
    }

    int FreeguildHandgunners::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
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
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar