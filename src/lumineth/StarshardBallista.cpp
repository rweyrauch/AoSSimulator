/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/StarshardBallista.h>
#include <UnitFactory.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 0;

    bool VanariStarshardBallistas::s_registered = false;

    Unit *VanariStarshardBallistas::Create(const ParameterList &parameters) {
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        return new VanariStarshardBallistas(nation);
    }

    int VanariStarshardBallistas::ComputePoints(int numModels) {
        return g_pointsPerUnit;
    }

    void VanariStarshardBallistas::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };
            s_registered = UnitFactory::Register("Vanari Starshard Ballistas", factoryMethod);
        }
    }

    VanariStarshardBallistas::VanariStarshardBallistas(GreatNation nation) :
            LuminethBase("Vanari Starshard Ballistas", 6, g_wounds, 6, 5, false) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, STARSHARD_BALLISTAS};
        m_weapons = {&m_bolts, &m_swords};
        m_battleFieldRole = Role::Artillery;

        setNation(nation);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bolts);
        model->addMeleeWeapon(&m_swords);
        addModel(model);

        m_points = ComputePoints(1);
    }
}