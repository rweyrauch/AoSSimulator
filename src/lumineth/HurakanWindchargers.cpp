/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/HurakanWindchargers.h>
#include <UnitFactory.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize/g_minUnitSize)*g_pointsPerBlock;

    bool HurakanWindchargers::s_registered = false;

    Unit *HurakanWindchargers::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        return new HurakanWindchargers(nation, numModels, ComputePoints(parameters));
    }

    int HurakanWindchargers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void HurakanWindchargers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Hurakan Windchargers", factoryMethod);
        }
    }

    HurakanWindchargers::HurakanWindchargers(GreatNation nation, int numModels, int points) :
        LuminethBase(nation, "Hurakan Windchargers", 14, g_wounds, 7, 5, false, points) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, AELEMENTIRI, HURAKAN, WINDCHARGERS};
        m_weapons = {&m_bow, &m_bowSeneschal, &m_bowMelee, &m_bowSeneschalMelee, &m_claws};
        m_battleFieldRole = Role::Other;

        auto seneschal = new Model(g_basesize, wounds());
        seneschal->addMissileWeapon(&m_bowSeneschal);
        seneschal->addMeleeWeapon(&m_bowSeneschalMelee);
        seneschal->addMeleeWeapon(&m_claws);
        seneschal->setName("Windspeaker Seneschal");
        addModel(seneschal);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_bow);
            model->addMeleeWeapon(&m_bowMelee);
            model->addMeleeWeapon(&m_claws);
            addModel(model);
        }
    }
}