/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Yndrasta.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 300;

    bool Yndrasta::s_registered = false;

    Yndrasta::Yndrasta(Stormhost stormhost, bool isGeneral) :
            StormcastEternal(stormhost, "Yndrasta", 12, g_wounds, 10, 3, true, g_pointsPerUnit) {
        m_keywords = {ORDER, STORMCAST_ETERNAL, THUNDERSTRIKE, HERO, YNDRASTRA};
        m_weapons = {&m_thengavar, &m_blade};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_thengavar);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Yndrasta::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Yndrasta(stormhost, general);
    }

    void Yndrasta::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Yndrasta", factoryMethod);
        }
    }

    int Yndrasta::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals