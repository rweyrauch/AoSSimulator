/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/TerradonRiders.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 50;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 360;

    bool TerradonRiders::s_registered = false;

    TerradonRiders::TerradonRiders(WayOfTheSeraphon way, Constellation constellation, int numModels, WeaponOption option, int points) :
            SeraphonBase("Terradon Riders", 16, g_wounds, 5, 6, true, points) {
        m_keywords = {ORDER, SERAPHON, SKINK, TERRADON, TERRADON_RIDERS};
        m_weapons = {&m_javelin, &m_javelinLeader, &m_bolas, &m_bolasLeader, &m_jaws};
        m_hasMount = true;
        m_jaws.setMount(true);

        setWayOfTheSeraphon(way, constellation);

        // Add the Alpha
        auto alpha = new Model(g_basesize, wounds());
        if (option == Starstrike_Javelins) {
            alpha->addMissileWeapon(&m_javelinLeader);
        } else if (option == Sunleech_Bolas) {
            alpha->addMissileWeapon(&m_bolasLeader);
        }
        alpha->addMeleeWeapon(&m_jaws);
        addModel(alpha);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (option == Starstrike_Javelins) {
                model->addMissileWeapon(&m_javelin);
            } else if (option == Sunleech_Bolas) {
                model->addMissileWeapon(&m_bolas);
            }
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }
    }

    Unit *TerradonRiders::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto option = (WeaponOption) GetEnumParam("Weapons", parameters, Starstrike_Javelins);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);

        return new TerradonRiders(way, constellation, numModels, option, ComputePoints(parameters));
    }

    void TerradonRiders::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Starstrike_Javelins, Sunleech_Bolas};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Starstrike_Javelins, weapons),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Terradon Riders", factoryMethod);
        }
    }

    int TerradonRiders::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    std::string TerradonRiders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Starstrike_Javelins) return "Starstrike Javelins";
            else if (parameter.intValue == Sunleech_Bolas) return "Sunleech Bolas";
        }
        return SeraphonBase::ValueToString(parameter);
    }

    int TerradonRiders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Starstrike Javelins") return Starstrike_Javelins;
        else if (enumString == "Sunleech Bolas") return Sunleech_Bolas;
        return SeraphonBase::EnumStringToInt(enumString);
    }

} //namespace Seraphon
