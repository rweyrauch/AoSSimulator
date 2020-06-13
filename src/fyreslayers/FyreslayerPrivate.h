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
            to_integer(Lodge::None),
            to_integer(Lodge::Vostarg),
            to_integer(Lodge::Greyfyrd),
            to_integer(Lodge::Hermdar),
            to_integer(Lodge::Lofnir)
    };

    constexpr std::array<int, 7> g_fatherSonTraits = {
           to_integer(CommandTrait::None),
           to_integer(CommandTrait::Fury_of_the_Fyreslayers),
           to_integer(CommandTrait::Honour_of_the_Ancestors),
           to_integer(CommandTrait::Spirit_of_Grimnir),
           to_integer(CommandTrait::Blood_of_the_Berserker),
           to_integer(CommandTrait::Iron_Will_of_the_Guardian),
           to_integer(CommandTrait::Destroyer_of_Foes)
    };

    constexpr std::array<int, 7> g_masterSmiterTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Fyremantle),
            to_integer(CommandTrait::Wisdom_and_Authority),
            to_integer(CommandTrait::Oathsayer),
            to_integer(CommandTrait::Ash_Beard),
            to_integer(CommandTrait::Fyresteel_Weaponsmith),
            to_integer(CommandTrait::Master_Priest)
    };

    // Heirlooms of the Lodge
    constexpr std::array<int, 7> g_heirloomArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Magnetised_Runes),
            to_integer(Artefact::Draught_of_Magmalt_Ale),
            to_integer(Artefact::Drakeslayer),
            to_integer(Artefact::Obsidian_Glowhelm),
            to_integer(Artefact::Shimmering_Blade),
            to_integer(Artefact::Axe_of_Grimnir)
    };

    // Artefacts of Wrath and Doom
    constexpr std::array<int, 4> g_wrathAndDoomArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Beastslayer),
            to_integer(Artefact::Bracers_of_Ember_Iron),
            to_integer(Artefact::Rune_of_Blazing_Fury)
    };

    // Artefacts of the Forge-temple
    constexpr std::array<int, 7> g_forgeTempleArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Salamander_Cloak),
            to_integer(Artefact::Ash_Cloud_Rune),
            to_integer(Artefact::Volatile_Brazier),
            to_integer(Artefact::Ancestor_Helm),
            to_integer(Artefact::Emberstone_Rune),
            to_integer(Artefact::Droth_Helm)
    };

    // Icons of Grimnir
    constexpr std::array<int, 4> g_iconArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Icon_of_the_Ancestors),
            to_integer(Artefact::Icon_of_Grimnirs_Condemnation),
            to_integer(Artefact::The_Nulsidian_Icon)
    };

    constexpr std::array<int, 7> g_mountTraits = {
        to_integer(MountTrait::None),

        // Magmadroth
        to_integer(MountTrait::Cinder_Crest_Youngblood),
        to_integer(MountTrait::Flame_Scale_Youngblood),
        to_integer(MountTrait::Fire_Claw_Adult),
        to_integer(MountTrait::Lava_Tongue_Adult),
        to_integer(MountTrait::Coal_Heart_Ancient),
        to_integer(MountTrait::Ash_Horn_Ancient)
    };

    constexpr std::array<int, 7> g_prayers = {
        to_integer(Prayer::None),

        to_integer(Prayer::Molten_Infusion),
        to_integer(Prayer::Searing_Heat),
        to_integer(Prayer::Prayer_of_Ash),
        to_integer(Prayer::Ember_Storm),
        to_integer(Prayer::Prayer_of_Grimnirs_Fury),
        to_integer(Prayer::Gilded_Claws)
    };

} // namespace Fyreslayers

#endif //FYRESLAYERPRIVATE_H