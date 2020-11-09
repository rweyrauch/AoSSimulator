/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FYRESLAYERPRIVATE_H
#define FYRESLAYERPRIVATE_H

#include <fyreslayers/Fyreslayer.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Fyreslayers {

    constexpr std::array<int, 5> g_lodge = {
            ToInteger(Lodge::None),
            ToInteger(Lodge::Vostarg),
            ToInteger(Lodge::Greyfyrd),
            ToInteger(Lodge::Hermdar),
            ToInteger(Lodge::Lofnir)
    };

    constexpr std::array<int, 7> g_fatherSonTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Fury_Of_The_Fyreslayers),
            ToInteger(CommandTrait::Honour_Of_The_Ancestors),
            ToInteger(CommandTrait::Spirit_Of_Grimnir),
            ToInteger(CommandTrait::Blood_Of_The_Berserker),
            ToInteger(CommandTrait::Iron_Will_Of_The_Guardian),
            ToInteger(CommandTrait::Destroyer_Of_Foes)
    };

    constexpr std::array<int, 7> g_masterSmiterTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Fyremantle),
            ToInteger(CommandTrait::Wisdom_And_Authority),
            ToInteger(CommandTrait::Oathsayer),
            ToInteger(CommandTrait::Ash_Beard),
            ToInteger(CommandTrait::Fyresteel_Weaponsmith),
            ToInteger(CommandTrait::Master_Priest)
    };

    // Heirlooms of the Lodge
    constexpr std::array<int, 7> g_heirloomArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Magnetised_Runes),
            ToInteger(Artefact::Draught_Of_Magmalt_Ale),
            ToInteger(Artefact::Drakeslayer),
            ToInteger(Artefact::Obsidian_Glowhelm),
            ToInteger(Artefact::Shimmering_Blade),
            ToInteger(Artefact::Axe_Of_Grimnir)
    };

    // Artefacts of Wrath and Doom
    constexpr std::array<int, 4> g_wrathAndDoomArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Beastslayer),
            ToInteger(Artefact::Bracers_Of_Ember_Iron),
            ToInteger(Artefact::Rune_Of_Blazing_Fury)
    };

    // Artefacts of the Forge-temple
    constexpr std::array<int, 7> g_forgeTempleArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Salamander_Cloak),
            ToInteger(Artefact::Ash_Cloud_Rune),
            ToInteger(Artefact::Volatile_Brazier),
            ToInteger(Artefact::Ancestor_Helm),
            ToInteger(Artefact::Emberstone_Rune),
            ToInteger(Artefact::Droth_Helm)
    };

    // Icons of Grimnir
    constexpr std::array<int, 4> g_iconArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Icon_Of_The_Ancestors),
            ToInteger(Artefact::Icon_Of_Grimnirs_Condemnation),
            ToInteger(Artefact::The_Nulsidian_Icon)
    };

    constexpr std::array<int, 7> g_mountTraits = {
            ToInteger(MountTrait::None),

        // Magmadroth
            ToInteger(MountTrait::Cinder_Crest_Youngblood),
            ToInteger(MountTrait::Flame_Scale_Youngblood),
            ToInteger(MountTrait::Fire_Claw_Adult),
            ToInteger(MountTrait::Lava_Tongue_Adult),
            ToInteger(MountTrait::Coal_Heart_Ancient),
            ToInteger(MountTrait::Ash_Horn_Ancient)
    };

    constexpr std::array<int, 7> g_prayers = {
            ToInteger(Prayer::None),

            ToInteger(Prayer::Molten_Infusion),
            ToInteger(Prayer::Searing_Heat),
            ToInteger(Prayer::Prayer_Of_Ash),
            ToInteger(Prayer::Ember_Storm),
            ToInteger(Prayer::Prayer_Of_Grimnirs_Fury),
            ToInteger(Prayer::Gilded_Claws)
    };

} // namespace Fyreslayers

#endif //FYRESLAYERPRIVATE_H