/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Avalenor.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int BASESIZE = 105; // oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 360;

    bool AvalenorTheStoneheartKing::s_registered = false;

    AvalenorTheStoneheartKing::AvalenorTheStoneheartKing() :
            LuminethBase("Avalenor, the Stoneheart King", 6, WOUNDS, 10, 3, false) {
        m_battleFieldRole = Role::LeaderBehemoth;
    }

    bool AvalenorTheStoneheartKing::configure() {
        return false;
    }

    Unit *AvalenorTheStoneheartKing::Create(const ParameterList &parameters) {
        auto unit = new AvalenorTheStoneheartKing();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AvalenorTheStoneheartKing::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Avalenor, the Stoneheart King", factoryMethod);
        }
    }

    int AvalenorTheStoneheartKing::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace LuminethRealmLords