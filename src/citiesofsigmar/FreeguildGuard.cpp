/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGuard.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool FreeguildGuard::s_registered = false;

    Unit *FreeguildGuard::Create(const ParameterList &parameters) {
        auto unit = new FreeguildGuard();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool drummer = GetBoolParam("Drummer", parameters, true);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Halberd);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, standard, drummer, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FreeguildGuard::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Halberd) {
                return "Halberd";
            } else if (parameter.intValue == Spear) {
                return "Spear";
            } else if (parameter.intValue == Sword) {
                return "Sword";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildGuard::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Halberd") {
            return Halberd;
        } else if (enumString == "Spear") {
            return Spear;
        } else if (enumString == "Sword") {
            return Sword;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildGuard::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Halberd, Spear, Sword};
            static FactoryMethod factoryMethod = {
                    FreeguildGuard::Create,
                    FreeguildGuard::ValueToString,
                    FreeguildGuard::EnumStringToInt,
                    FreeguildGuard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Spear, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Freeguild Guard", factoryMethod);
        }
    }

    FreeguildGuard::FreeguildGuard() :
            CitizenOfSigmar("Freeguild Guard", 5, g_wounds, 5, 5, false),
            m_halberd(Weapon::Type::Melee, "Freeguild Halberd", 1, 1, 4, 3, -1, 1),
            m_spear(Weapon::Type::Melee, "Freeguild Spear", 2, 1, 4, 4, 0, 1),
            m_sword(Weapon::Type::Melee, "Freeguild Sword", 1, 1, 4, 4, 0, 1),
            m_halberdSergeant(Weapon::Type::Melee, "Freeguild Halberd", 1, 2, 4, 3, -1, 1),
            m_spearSergeant(Weapon::Type::Melee, "Freeguild Spear", 2, 2, 4, 4, 0, 1),
            m_swordSergeant(Weapon::Type::Melee, "Freeguild Sword", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_GUARD};
        m_weapons = {&m_halberd, &m_spear, &m_sword, &m_halberdSergeant, &m_spearSergeant, &m_swordSergeant};
        m_battleFieldRole = Role::Battleline;
    }

    bool FreeguildGuard::configure(int numModels, bool standardBearer, bool drummer, WeaponOption weapons) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Sergeant
        auto bossModel = new Model(g_basesize, wounds());
        if (weapons == Halberd) {
            bossModel->addMeleeWeapon(&m_halberdSergeant);
        } else if (weapons == Spear) {
            bossModel->addMeleeWeapon(&m_spearSergeant);
        } else if (weapons == Sword) {
            bossModel->addMeleeWeapon(&m_swordSergeant);
        }
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Halberd) {
                model->addMeleeWeapon(&m_halberd);
            } else if (weapons == Spear) {
                model->addMeleeWeapon(&m_spearSergeant);
            } else if (weapons == Sword) {
                model->addMeleeWeapon(&m_sword);
            }
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (drummer) {
                model->setName(Model::Drummer);
                drummer = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int FreeguildGuard::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Drummer)) mod++;
        return mod;
    }

    int FreeguildGuard::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Drummer)) mod++;
        return mod;
    }

    int FreeguildGuard::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int FreeguildGuard::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);

        // Massed Ranks
        if (remainingModels() >= 20) mod += 2;
        else if (remainingModels() >= 10) mod++;

        // Wall of Spears
        if (target->charged() && (weapon->name() == m_spear.name())) mod++;

        return mod;
    }

    int FreeguildGuard::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = CitizenOfSigmar::toSaveModifier(weapon, attacker);

        // Parry and Block
        if (weapon->name() == m_sword.name()) mod++;

        return mod;
    }

    int FreeguildGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar