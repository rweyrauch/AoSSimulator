/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/LyriorUthralle.h>
#include <UnitFactory.h>

namespace LuminethRealmLords {

    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool LyriorUthralle::s_registered = false;

    Unit *LyriorUthralle::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new LyriorUthralle(general);
    }

    int LyriorUthralle::ComputePoints(int numModels) {
        return g_pointsPerUnit;
    }

    void LyriorUthralle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };
            s_registered = UnitFactory::Register("Lyrior Uthralle", factoryMethod);
        }
    }

    LyriorUthralle::LyriorUthralle(bool isGeneral) :
            LuminethBase("Lyrior Uthralle", 16, g_wounds, 9, 3, false) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, HERO, WIZARD, LORD_REGENT, Sunmetal_Weapons};
        m_weapons = {&m_daemonbane, &m_daemonbaneMelee, &m_sword, &m_hornsAndClaws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hornsAndClaws.setMount(true);

        setNation(GreatNation::Ymetrica);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_daemonbane);
        model->addMeleeWeapon(&m_daemonbaneMelee);
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_hornsAndClaws);
        addModel(model);

        m_points = ComputePoints(1);
    }
}