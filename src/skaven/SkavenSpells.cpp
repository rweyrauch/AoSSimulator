/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <skaven/SkavenSpells.h>
#include <Board.h>
#include <Unit.h>

namespace Skaven {

    DamageSpell *CreateWarpLightning(Unit *caster) {
        // TODO: Implement the 'warp-power accumulator' option.
        return new DamageSpell(caster, "Warp Lightning", 5, 13, RAND_D3);
    }

    AreaOfEffectSpell *CreateWarpLightningStorm(Unit *caster) {
        // TODO: Implement the 'warp-power accumulator' option.
        // TODO: Storm is limited to D3 units (really D3 casts of WarpLightning)
        return new AreaOfEffectSpell(caster, "Warp Lightning Storm", 7, 0, 13, RAND_D3, 0);
    }

    Spell* CreateLore(Lore which, Unit* caster) {
        switch (which) {
            case Lore::Scorch:
                return nullptr;
            case Lore::Splinter:
                return nullptr;
            case Lore::Skitterleap:
                return nullptr;
            case Lore::Plague:
                return nullptr;
            case Lore::Death_Frenzy:
                return nullptr;
            case Lore::Warpgale:
                return nullptr;
            case Lore::More_More_More_Warp_Power:
                return nullptr;
            case Lore::Chain_Warp_Lightning:
                return nullptr;
            case Lore::Warp_Lighning_Shield:
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }
}

