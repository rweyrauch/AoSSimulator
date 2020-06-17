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
    static const int BASESIZE = 60;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    bool FreeguildOutriders::s_registered = false;

    Unit *FreeguildOutriders::Create(const ParameterList &parameters) {
        auto unit = new FreeguildOutriders();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool trumpeter = GetBoolParam("Trumpeter", parameters, true);
        auto sharpshooterWeapon = (WeaponOption) GetEnumParam("Sharpshooter Weapon", parameters, RepeaterHandgun);

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
            if (parameter.intValue == RepeaterHandgun) {
                return "Repeater Handgun";
            } else if (parameter.intValue == Blunderbuss) {
                return "Blunderbuss";
            } else if (parameter.intValue == BraceOfPistols) {
                return "Brace of Pistols";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildOutriders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Repeater Handgun") {
            return RepeaterHandgun;
        } else if (enumString == "Blunderbuss") {
            return Blunderbuss;
        } else if (enumString == "Brace of Pistols") {
            return BraceOfPistols;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildOutriders::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {RepeaterHandgun, Blunderbuss, BraceOfPistols};
            static FactoryMethod factoryMethod = {
                    FreeguildOutriders::Create,
                    FreeguildOutriders::ValueToString,
                    FreeguildOutriders::EnumStringToInt,
                    FreeguildOutriders::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Sharpshooter Weapon", RepeaterHandgun, weapons),
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
            CitizenOfSigmar("Freeguild Outriders", 12, WOUNDS, 6, 5, false),
            m_blunderbuss(Weapon::Type::Missile, "Grenade-launching Blunderbuss", 12, 1, 4, 3, -1, RAND_D3),
            m_pistols(Weapon::Type::Missile, "Brace of Pistols", 9, 2, 3, 3, -1, 1),
            m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 5, 3, -1, 1),
            m_sabre(Weapon::Type::Melee, "Freeguild Cavalry Sabre", 1, 1, 4, 4, 0, 1),
            m_hooves(Weapon::Type::Melee, "Stamping Hooves", 1, 2, 4, 5, 0, 1),
            m_sabreSharpshooter(Weapon::Type::Melee, "Freeguild Cavalry Sabre", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_OUTRIDERS};
        m_weapons = {&m_blunderbuss, &m_pistols, &m_handgun, &m_sabre, &m_hooves, &m_sabreSharpshooter};

        // Skilled Riders
        m_runAndShoot = true;
        m_retreatAndShoot = true;
    }

    bool FreeguildOutriders::configure(int numModels, bool trumpeter, WeaponOption sharpshooterWeapon) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_trumpeter = trumpeter;

        // Add the Sharpshooter
        auto bossModel = new Model(BASESIZE, wounds());
        if (sharpshooterWeapon == RepeaterHandgun) {
            bossModel->addMissileWeapon(&m_handgun);
        } else if (sharpshooterWeapon == Blunderbuss) {
            bossModel->addMissileWeapon(&m_blunderbuss);
        } else if (sharpshooterWeapon == BraceOfPistols) {
            bossModel->addMeleeWeapon(&m_pistols);
        }
        bossModel->addMeleeWeapon(&m_sabreSharpshooter);
        bossModel->addMeleeWeapon(&m_hooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_handgun);
            model->addMeleeWeapon(&m_sabre);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int FreeguildOutriders::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_trumpeter) mod++;
        return mod;
    }

    int FreeguildOutriders::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_trumpeter) mod++;
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar