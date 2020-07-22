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
    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 190;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 3;

    bool MorghastArchai::s_registered = false;

    MorghastArchai::MorghastArchai() :
            OssiarchBonereaperBase("Morghast Archai", 9, WOUNDS, 10, 4, true),
            m_spiritHalberd(Weapon::Type::Melee, "Spirit Halberd", 2, 3, 3, 3, -2, 3),
            m_spiritSwords(Weapon::Type::Melee, "Spirit Swords", 1, 5, 3, 3, -1, 2) {
        m_keywords = {DEATH, MORGHAST, DEATHLORDS, OSSIARCH_BONEREAPERS, HEKATOS, MORGHAST_ARCHAI};
        m_weapons = {&m_spiritHalberd, &m_spiritSwords};
    }

    bool MorghastArchai::configure(int numModels, WeaponOptions weapons) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_weaponOption = weapons;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == SpiritHalberd) {
                model->addMeleeWeapon(&m_spiritHalberd);
            } else if (weapons == SpiritSwords) {
                model->addMeleeWeapon(&m_spiritSwords);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *MorghastArchai::Create(const ParameterList &parameters) {
        auto unit = new MorghastArchai();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, SpiritHalberd);

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
            if (parameter.intValue == SpiritHalberd) { return "Spirit Halberd"; }
            else if (parameter.intValue == SpiritSwords) { return "Spirit Swords"; }
        }
        return ParameterValueToString(parameter);
    }

    int MorghastArchai::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Spirit Halberd") { return SpiritHalberd; }
        else if (enumString == "Spirit Swords") { return SpiritSwords; }
        return 0;
    }

    void MorghastArchai::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {SpiritHalberd, SpiritSwords};
            static FactoryMethod factoryMethod = {
                    MorghastArchai::Create,
                    MorghastArchai::ValueToString,
                    MorghastArchai::EnumStringToInt,
                    MorghastArchai::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", SpiritHalberd, weapons),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Morghast Archai", factoryMethod);
        }
    }

    Wounds MorghastArchai::applyWoundSave(const Wounds &wounds) {
        // Ebon-wrought Armour
        Dice::RollResult mortalSaves;
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int MorghastArchai::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace OssiarchBonereapers