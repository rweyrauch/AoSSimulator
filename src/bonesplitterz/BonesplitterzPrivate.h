/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <bonesplitterz/Bonesplitterz.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Bonesplitterz {

    constexpr std::array<int, 4> g_warclan = {
            ToInteger(Warclan::None),
            ToInteger(Warclan::Bonegrinz),
            ToInteger(Warclan::Drakkfoot),
            ToInteger(Warclan::Icebone)
    };

    constexpr std::array<int, 7> g_prophetBossCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Killa_Instinkt),
            ToInteger(CommandTrait::Waaagh_Monger),
            ToInteger(CommandTrait::Great_Hunter),
            ToInteger(CommandTrait::Power_Of_The_Beast),
            ToInteger(CommandTrait::Voice_Of_Da_Gods),
            ToInteger(CommandTrait::Monsta_Killa)
    };

    constexpr std::array<int, 4> g_wizardCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Dead_Kunnin),
            ToInteger(CommandTrait::Master_Of_The_Weird),
            ToInteger(CommandTrait::Fuelled_By_The_Spirits)
    };

    constexpr std::array<int, 7> g_heroArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Dokk_Juice),
            ToInteger(Artefact::Savage_Trophy),
            ToInteger(Artefact::Lucky_Bone),
            ToInteger(Artefact::Glowin_Tattooz),
            ToInteger(Artefact::Greatdrake_Toof),
            ToInteger(Artefact::Weepwood_Big_Shiv)
    };

    constexpr std::array<int, 4> g_wizardArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Big_Wurrgog_Mask),
            ToInteger(Artefact::Morks_Boney_Bitz),
            ToInteger(Artefact::Mystic_Waaagh_Paint)
    };

    constexpr std::array<int, 7> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Squiggly_Curse),
            ToInteger(Lore::Breath_Of_Gorkamorka),
            ToInteger(Lore::Brutal_Beast_Spirits),
            ToInteger(Lore::Bone_Krusha),
            ToInteger(Lore::Kunnin_Beast_Spirits),
            ToInteger(Lore::Gorkamorkas_War_Cry)
    };

} // namespace Bonesplitterz
