/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Sevireth.h>
#include <UnitFactory.h>

namespace LuminethRealmLords {

    static const int g_basesize = 40;
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 0;

    bool Sevireth::s_registered = false;

    Unit *Sevireth::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new Sevireth(general);
    }

    int Sevireth::ComputePoints(int numModels) {
        return g_pointsPerUnit;
    }

    void Sevireth::Init() {
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
            s_registered = UnitFactory::Register("Sevireth, Lord of the Seventh Wind", factoryMethod);
        }
    }

    Sevireth::Sevireth(bool isGeneral) :
            LuminethBase("Sevireth, Lord of the Seventh Wind", 24, g_wounds, 10, 5, true) {
        m_keywords = {ORDER, LUMINETH_REALM_LORDS, HURAKAN, YMETRICA, HERO, SPIRIT_OF_THE_WIND, SEVIRETH};
        m_weapons = {&m_bow, &m_bowMelee, &m_shards};
        m_battleFieldRole = Role::Leader;

        setNation(GreatNation::Ymetrica);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bow);
        model->addMeleeWeapon(&m_bowMelee);
        model->addMeleeWeapon(&m_shards);
        addModel(model);

        m_points = ComputePoints(1);
    }
}