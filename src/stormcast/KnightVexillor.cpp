/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightVexillor.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 110;

    bool KnightVexillor::s_registered = false;

    KnightVexillor::KnightVexillor() :
            StormcastEternal("Knight-Vexillor", 5, g_wounds, 8, 3, false),
            m_warhammer(Weapon::Type::Melee, "Warhammer", 1, 4, 4, 3, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_VEXILLOR};
        m_weapons = {&m_warhammer};
        m_battleFieldRole = Role::Leader;

        s_globalChargeReroll.connect(this, &KnightVexillor::iconOfWarChargeReroll, &m_iconOfWarSlot);
    }

    KnightVexillor::~KnightVexillor() {
        m_iconOfWarSlot.disconnect();
    }

    bool KnightVexillor::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_warhammer);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *KnightVexillor::Create(const ParameterList &parameters) {
        auto unit = new KnightVexillor();

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KnightVexillor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Vexillor", factoryMethod);
        }
    }

    Rerolls KnightVexillor::iconOfWarChargeReroll(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(STORMCAST_ETERNAL) && (distanceTo(unit) <= 18.0)) return Rerolls::Failed;
        return Rerolls::None;
    }

    int KnightVexillor::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals