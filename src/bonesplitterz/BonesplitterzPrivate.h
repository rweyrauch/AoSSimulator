/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BONESPLITTERZPRIVATE_H
#define BONESPLITTERZPRIVATE_H

#include <bonesplitterz/Bonesplitterz.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Bonesplitterz {

    constexpr std::array<int, 4> g_warclan = {
            to_integer(Warclan::None),
            to_integer(Warclan::Bonegrinz),
            to_integer(Warclan::Drakkfoot),
            to_integer(Warclan::Icebone)
    };

    constexpr std::array<int, 7> g_prophetBossCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Killa_Instinkt),
            to_integer(CommandTrait::Waaagh_Monger),
            to_integer(CommandTrait::Great_Hunter),
            to_integer(CommandTrait::Power_of_the_Beast),
            to_integer(CommandTrait::Voice_of_Da_Gods),
            to_integer(CommandTrait::Monsta_Killa)
    };

    constexpr std::array<int, 4> g_wizardCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Dead_Kunnin),
            to_integer(CommandTrait::Master_of_the_Weird),
            to_integer(CommandTrait::Fuelled_by_the_Spirits)
    };

    constexpr std::array<int, 7> g_heroArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Dokk_Juice),
            to_integer(Artefact::Savage_Trophy),
            to_integer(Artefact::Lucky_Bone),
            to_integer(Artefact::Glowin_Tattooz),
            to_integer(Artefact::Greatdrake_Toof),
            to_integer(Artefact::Weepwood_Big_Shiv)
    };

    constexpr std::array<int, 4> g_wizardArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Big_Wurrgog_Mask),
            to_integer(Artefact::Morks_Boney_Bitz),
            to_integer(Artefact::Mystic_Waaagh_Paint)
    };

    constexpr std::array<int, 7> g_lore = {
        to_integer(Lore::None),
        to_integer(Lore::Squiggly_Curse),
        to_integer(Lore::Breath_of_Gorkamorka),
        to_integer(Lore::Brutal_Beast_Spirits),
        to_integer(Lore::Bone_Krusha),
        to_integer(Lore::Kunnin_Beast_Spirits),
        to_integer(Lore::Gorkamorkas_War_Cry)
    };

} // namespace Bonesplitterz

#endif // BONESPLITTERZPRIVATE_H