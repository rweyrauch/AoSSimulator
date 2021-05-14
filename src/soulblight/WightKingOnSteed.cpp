/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/WightKingOnSteed.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 0;

    bool WightKingOnSteed::s_registered = false;

    Unit *WightKingOnSteed::Create(const ParameterList &parameters) {
        return nullptr;
    }

    int WightKingOnSteed::ComputePoints(const ParameterList &parameters) {
        return 0;
    }

    void WightKingOnSteed::Init() {

    }

    WightKingOnSteed::WightKingOnSteed(CursedBloodline legion, CommandTrait trait, Artefact artefact, bool isGeneral) :
        SoulblightBase(legion, "Wight King on Skeletal Steed", 12, g_wounds, 10, 3, true, g_pointsPerUnit) {

    }

    Wounds WightKingOnSteed::applyWoundSave(const Wounds &wounds, Unit *attackingUnitk) {
        return Unit::applyWoundSave(wounds, attackingUnitk);
    }

    void WightKingOnSteed::onStartHero(PlayerId player) {
        EventInterface::onStartHero(player);
    }
} // namespace Soulblight
