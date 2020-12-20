/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/FreeguildOutriders.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 60;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool FreeguildOutriders::s_registered = false;

    Unit *FreeguildOutriders::Create(const ParameterList &parameters) {
        auto unit = new FreeguildOutriders();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool trumpeter = GetBoolParam("Trumpeter", parameters, true);
        auto sharpshooterWeapon = (WeaponOption) GetEnumParam("Sharpshooter Weapon", parameters, Repeater_Handgun);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, trumpeter, sharpshooterWeapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FreeguildOutriders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Sharpshooter Weapon") {
            if (parameter.intValue == Repeater_Handgun) {
                return "Repeater Handgun";
            } else if (parameter.intValue == Blunderbuss) {
                return "Blunderbuss";
            } else if (parameter.intValue == Brace_Of_Pistols) {
                return "Brace of Pistols";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildOutriders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Repeater Handgun") {
            return Repeater_Handgun;
        } else if (enumString == "Blunderbuss") {
            return Blunderbuss;
        } else if (enumString == "Brace of Pistols") {
            return Brace_Of_Pistols;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildOutriders::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Repeater_Handgun, Blunderbuss, Brace_Of_Pistols};
            static FactoryMethod factoryMethod = {
                    FreeguildOutriders::Create,
                    FreeguildOutriders::ValueToString,
                    FreeguildOutriders::EnumStringToInt,
                    FreeguildOutriders::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Sharpshooter Weapon", Repeater_Handgun, weapons),
                            BoolParameter("Trumpeter"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Freeguild Outriders", factoryMethod);
        }
    }

    FreeguildOutriders::FreeguildOutriders() :
            CitizenOfSigmar("Freeguild Outriders", 12, g_wounds, 6, 5, false),
            m_blunderbuss(Weapon::Type::Missile, "Grenade-launching Blunderbuss", 12, 1, 4, 3, -1, RAND_D3),
            m_pistols(Weapon::Type::Missile, "Brace of Pistols", 9, 2, 3, 3, -1, 1),
            m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 5, 3, -1, 1),
            m_sabre(Weapon::Type::Melee, "Freeguild Cavalry Sabre", 1, 1, 4, 4, 0, 1),
            m_hooves(Weapon::Type::Melee, "Stamping Hooves", 1, 2, 4, 5, 0, 1),
            m_sabreSharpshooter(Weapon::Type::Melee, "Freeguild Cavalry Sabre", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_OUTRIDERS};
        m_weapons = {&m_blunderbuss, &m_pistols, &m_handgun, &m_sabre, &m_hooves, &m_sabreSharpshooter};
        m_hasMount = true;

        // Skilled Riders
        m_runAndShoot = true;
        m_retreatAndShoot = true;
    }

    bool FreeguildOutriders::configure(int numModels, bool trumpeter, WeaponOption sharpshooterWeapon) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Sharpshooter
        auto bossModel = new Model(g_basesize, wounds());
        if (sharpshooterWeapon == Repeater_Handgun) {
            bossModel->addMissileWeapon(&m_handgun);
        } else if (sharpshooterWeapon == Blunderbuss) {
            bossModel->addMissileWeapon(&m_blunderbuss);
        } else if (sharpshooterWeapon == Brace_Of_Pistols) {
            bossModel->addMeleeWeapon(&m_pistols);
        }
        bossModel->addMeleeWeapon(&m_sabreSharpshooter);
        bossModel->addMeleeWeapon(&m_hooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_handgun);
            model->addMeleeWeapon(&m_sabre);
            model->addMeleeWeapon(&m_hooves);
            if (trumpeter) {
                model->setName(Model::Trumpeter);
                trumpeter = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int FreeguildOutriders::runModifier() const {
        auto mod = Unit::runModifier();
        if (isNamedModelAlive(Model::Trumpeter)) mod++;
        return mod;
    }

    int FreeguildOutriders::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (isNamedModelAlive(Model::Trumpeter)) mod++;
        return mod;
    }

    int FreeguildOutriders::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extras = Unit::extraAttacks(attackingModel, weapon, target);
        // Expert Gunners
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        if ((weapon->name() == m_handgun.name()) && units.empty()) {
            extras++;
        }
        return extras;
    }

    int FreeguildOutriders::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar