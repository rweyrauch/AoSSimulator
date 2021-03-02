/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

namespace Abilities {

    enum class Target {
        None,
        Any,
        Point,
        Friendly,
        Enemy,
        Self,
        SelfAndFriendly
    };

    enum class EffectType {
        Utility,
        Damage,
        AreaOfEffectDamage,
        Heal,
        Buff,
        Debuff,
        Summon
    };
}