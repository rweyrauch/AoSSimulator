/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Eltharion.h>
#include <UnitFactory.h>
#include <iostream>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 0;

    bool TheLightOfEltharion::s_registered = false;

    TheLightOfEltharion::TheLightOfEltharion() :
            LuminethBase("The Light of Eltharion", 6, WOUNDS, 10, 3, false),
            m_fangsword(Weapon::Type::Melee, "Fangsword of Eltharion", 1, 4, 2, 3, -3, RAND_D3),
            m_blade(Weapon::Type::Melee, "Celennari Blade", 1, 2, 2, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, HERO, LIGHT_OF_ELTHARION};
        m_weapons = {&m_fangsword, &m_blade};
        m_battleFieldRole = Role::Leader;
    }

    bool TheLightOfEltharion::configure() {

        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_fangsword);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *TheLightOfEltharion::Create(const ParameterList &parameters) {
        auto unit = new TheLightOfEltharion();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TheLightOfEltharion::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("The Light of Eltharion", factoryMethod);
        }
    }

    int TheLightOfEltharion::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace LuminethRealmLords