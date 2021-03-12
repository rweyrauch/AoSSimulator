/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <ossiarch/MorghastArchai.h>
#include <UnitFactory.h>
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 60;
    static const int g_wounds = 6;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool MorghastArchai::s_registered = false;

    MorghastArchai::MorghastArchai() :
            OssiarchBonereaperBase("Morghast Archai", 9, g_wounds, 10, 4, true),
            m_spiritHalberd(Weapon::Type::Melee, "Spirit Halberd", 2, 3, 3, 3, -2, 3),
            m_spiritSwords(Weapon::Type::Melee, "Spirit Swords", 1, 5, 3, 3, -1, 2) {
        m_keywords = {DEATH, MORGHAST, DEATHLORDS, OSSIARCH_BONEREAPERS, HEKATOS, MORGHAST_ARCHAI};
        m_weapons = {&m_spiritHalberd, &m_spiritSwords};
    }

    bool MorghastArchai::configure(int numModels, WeaponOptions weapons) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        m_weaponOption = weapons;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Spirit_Halberd) {
                model->addMeleeWeapon(&m_spiritHalberd);
            } else if (weapons == Spirit_Swords) {
                model->addMeleeWeapon(&m_spiritSwords);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *MorghastArchai::Create(const ParameterList &parameters) {
        auto unit = new MorghastArchai();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Spirit_Halberd);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string MorghastArchai::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Spirit_Halberd) { return "Spirit Halberd"; }
            else if (parameter.intValue == Spirit_Swords) { return "Spirit Swords"; }
        }
        return ParameterValueToString(parameter);
    }

    int MorghastArchai::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Spirit Halberd") { return Spirit_Halberd; }
        else if (enumString == "Spirit Swords") { return Spirit_Swords; }
        return 0;
    }

    void MorghastArchai::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Spirit_Halberd, Spirit_Swords};
            static FactoryMethod factoryMethod = {
                    MorghastArchai::Create,
                    MorghastArchai::ValueToString,
                    MorghastArchai::EnumStringToInt,
                    MorghastArchai::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Spirit_Halberd, weapons),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Morghast Archai", factoryMethod);
        }
    }

    Wounds MorghastArchai::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Ebon-wrought Armour
        Dice::RollResult mortalSaves;
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int MorghastArchai::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace OssiarchBonereapers