/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/AkhelianAllopexes.h>
#include <UnitFactory.h>
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 8;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 330;

    bool AkhelianAllopexes::s_registered = false;

    AkhelianAllopexes::AkhelianAllopexes(int points) :
            IdonethDeepkinBase("Akhelian Alloplexes", 14, g_wounds, 6, 4, true, points),
            m_harpoonLauncher(Weapon::Type::Missile, "Razorshell Harpoon Launcher", 24, 4, 3, 3, -1, 1),
            m_netLauncher(Weapon::Type::Missile, "Retarius Net Launcher", 18, 1, 3, 3, 0, 3),
            m_hooksAndBlades(Weapon::Type::Melee, "Barbed Hooks and Blades", 1, 5, 3, 4, 0, 1),
            m_allopexBite(Weapon::Type::Melee, "Ferocious Bite", 1, 3, 3, 3, -2, 2) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, AKHELIAN, MONSTER, ALLOPEX};
        m_weapons = {&m_harpoonLauncher, &m_netLauncher, &m_hooksAndBlades, &m_allopexBite};
        m_hasMount = true;
        m_allopexBite.setMount(true);
    }

    bool AkhelianAllopexes::configure(int numModels, WeaponOption weapons) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Harpoon_Launcher) {
                model->addMissileWeapon(&m_harpoonLauncher);
            } else if (weapons == Net_Launcher) {
                model->addMissileWeapon(&m_netLauncher);
            }
            model->addMeleeWeapon(&m_hooksAndBlades);
            model->addMeleeWeapon(&m_allopexBite);
            model->addMeleeWeapon(&m_allopexFins);
            addModel(model);
        }

        return true;
    }

    Unit *AkhelianAllopexes::Create(const ParameterList &parameters) {
        auto unit = new AkhelianAllopexes(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Harpoon_Launcher);

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AkhelianAllopexes::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Harpoon_Launcher, Net_Launcher};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Harpoon_Launcher, weapons),
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Akhelian Alloplexes", factoryMethod);
        }
    }

    std::string AkhelianAllopexes::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Harpoon_Launcher) {
                return "Razorshell Harpoon Launcher";
            } else if (parameter.intValue == Net_Launcher) {
                return "Retarius Net Launcher";
            }
        }
        return IdonethDeepkinBase::ValueToString(parameter);
    }

    int AkhelianAllopexes::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Razorshell Harpoon Launcher") {
            return Harpoon_Launcher;
        } else if (enumString == "Retarius Net Launcher") {
            return Net_Launcher;
        }
        return IdonethDeepkinBase::EnumStringToInt(enumString);
    }

    int AkhelianAllopexes::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace IdonethDeepkin