/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildPistoliers.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 60;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool FreeguildPistoliers::s_registered = false;

    Unit *FreeguildPistoliers::Create(const ParameterList &parameters) {
        auto unit = new FreeguildPistoliers();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool trumpeter = GetBoolParam("Trumpeter", parameters, true);
        auto outriderWeapon = (WeaponOption) GetEnumParam("Outrider Weapon", parameters, Repeater_Handgun);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, trumpeter, outriderWeapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string FreeguildPistoliers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Outrider Weapon") {
            if (parameter.intValue == Repeater_Handgun) {
                return "Repeater Handgun";
            } else if (parameter.intValue == Brace_Of_Pistols) {
                return "Brace of Pistols";
            }
        }
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildPistoliers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Repeater Handgun") {
            return Repeater_Handgun;
        } else if (enumString == "Brace of Pistols") {
            return Brace_Of_Pistols;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildPistoliers::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Repeater_Handgun, Brace_Of_Pistols};
            static FactoryMethod factoryMethod = {
                    FreeguildPistoliers::Create,
                    FreeguildPistoliers::ValueToString,
                    FreeguildPistoliers::EnumStringToInt,
                    FreeguildPistoliers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter<2>("Outrider Weapon", Repeater_Handgun, weapons),
                            BoolParameter("Trumpeter"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Freeguild Pistoliers", factoryMethod);
        }
    }

    FreeguildPistoliers::FreeguildPistoliers() :
            CitizenOfSigmar("Freeguild Pistoliers", 12, g_wounds, 5, 5, false),
            m_pistols(Weapon::Type::Missile, "Brace of Pistols", 9, 2, 4, 3, -1, 1),
            m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
            m_sabreAndPistolButt(Weapon::Type::Melee, "Sabre and Pistol Butt", 1, 2, 4, 4, 0, 1),
            m_hooves(Weapon::Type::Melee, "Stamping Hooves", 1, 2, 4, 5, 0, 1),
            m_sabreAndPistolButtOutrider(Weapon::Type::Melee, "Sabre and Pistol Butt", 1, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_PISTOLIERS};
        m_weapons = {&m_pistols, &m_handgun, &m_sabreAndPistolButt, &m_hooves, &m_sabreAndPistolButtOutrider};
        m_hasMount = true;
        m_hooves.setMount(true);
    }

    bool FreeguildPistoliers::configure(int numModels, bool trumpeter, WeaponOption outriderWeapon) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Sharpshooter
        auto bossModel = new Model(g_basesize, wounds());
        if (outriderWeapon == Repeater_Handgun) {
            bossModel->addMissileWeapon(&m_handgun);
        } else if (outriderWeapon == Brace_Of_Pistols) {
            bossModel->addMeleeWeapon(&m_pistols);
        }
        bossModel->addMeleeWeapon(&m_sabreAndPistolButtOutrider);
        bossModel->addMeleeWeapon(&m_hooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_pistols);
            model->addMeleeWeapon(&m_sabreAndPistolButt);
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

    int FreeguildPistoliers::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Trumpeter)) mod++;
        return mod;
    }

    int FreeguildPistoliers::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Trumpeter)) mod++;
        return mod;
    }

    Rerolls FreeguildPistoliers::runRerolls() const {
        // Reckless Riders
        return Reroll_Failed;
    }

    Rerolls FreeguildPistoliers::chargeRerolls() const {
        // Reckless Riders
        return Reroll_Failed;
    }

    int FreeguildPistoliers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar