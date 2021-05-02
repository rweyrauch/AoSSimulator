/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <ossiarch/MorghastHarbingers.h>
#include <UnitFactory.h>
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 60;
    static const int g_wounds = 6;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool MorghastHarbingers::s_registered = false;

    MorghastHarbingers::MorghastHarbingers(Legion legion, int numModels, WeaponOptions weapons, int points) :
            OssiarchBonereaperBase("Morghast Harbingers", 9, g_wounds, 10, 4, true, points) {
        m_keywords = {DEATH, MORGHAST, DEATHLORDS, OSSIARCH_BONEREAPERS, HEKATOS, MORGHAST_HARBINGERS};
        m_weapons = {&m_spiritHalberd, &m_spiritSwords};

        setLegion(legion);

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
    }

    Unit *MorghastHarbingers::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Spirit_Halberd);
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);

        return new MorghastHarbingers(legion, numModels, weapons, ComputePoints(parameters));
    }

    std::string MorghastHarbingers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Spirit_Halberd) { return "Spirit Halberd"; }
            else if (parameter.intValue == Spirit_Swords) { return "Spirit Swords"; }
        }
        return ParameterValueToString(parameter);
    }

    int MorghastHarbingers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Spirit Halberd") { return Spirit_Halberd; }
        else if (enumString == "Spirit Swords") { return Spirit_Swords; }
        return 0;
    }

    void MorghastHarbingers::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Spirit_Halberd, Spirit_Swords};
            static FactoryMethod factoryMethod = {
                    MorghastHarbingers::Create,
                    MorghastHarbingers::ValueToString,
                    MorghastHarbingers::EnumStringToInt,
                    MorghastHarbingers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Spirit_Halberd, weapons),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Morghast Harbingers", factoryMethod);
        }
    }

    int MorghastHarbingers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace OssiarchBonereapers