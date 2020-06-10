/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <ossiarch/MorghastHarbingers.h>
#include <UnitFactory.h>

namespace OssiarchBonereapers {
    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 210;
    static const int POINTS_MAX_UNIT_SIZE = 630;

    bool MorghastHarbingers::s_registered = false;

    MorghastHarbingers::MorghastHarbingers() :
            OssiarchBonereaperBase("Morghast Harbingers", 9, WOUNDS, 10, 4, true),
            m_spiritHalberd(Weapon::Type::Melee, "Spirit Halberd", 2, 3, 3, 3, -2, 3),
            m_spiritSwords(Weapon::Type::Melee, "Spirit Swords", 1, 5, 3, 3, -1, 2) {
        m_keywords = {DEATH, MORGHAST, DEATHLORDS, OSSIARCH_BONEREAPERS, HEKATOS, MORGHAST_HARBINGERS};
        m_weapons = {&m_spiritHalberd, &m_spiritSwords};
    }

    bool MorghastHarbingers::configure(int numModels, WeaponOptions weapons) {
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

    Unit *MorghastHarbingers::Create(const ParameterList &parameters) {
        auto unit = new MorghastHarbingers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, SpiritHalberd);

        auto legion = (Legion) GetEnumParam("Legion", parameters, NoLegion);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string MorghastHarbingers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == SpiritHalberd) { return "Spirit Halberd"; }
            else if (parameter.intValue == SpiritSwords) { return "Spirit Swords"; }
        }
        return ParameterValueToString(parameter);
    }

    int MorghastHarbingers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Spirit Halberd") { return SpiritHalberd; }
        else if (enumString == "Spirit Swords") { return SpiritSwords; }
        return 0;
    }

    void MorghastHarbingers::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {SpiritHalberd, SpiritSwords};
            static FactoryMethod factoryMethod = {
                    MorghastHarbingers::Create,
                    MorghastHarbingers::ValueToString,
                    MorghastHarbingers::EnumStringToInt,
                    MorghastHarbingers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", SpiritHalberd, weapons),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Morghast Harbingers", factoryMethod);
        }
    }

    int MorghastHarbingers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace OssiarchBonereapers