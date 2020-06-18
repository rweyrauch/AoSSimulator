/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONJAWZPRIVATE_H
#define IRONJAWZPRIVATE_H

#include <ironjawz/Ironjawz.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Ironjawz {

    constexpr std::array<int, 4> g_warclan = {
            to_integer(Warclan::None),
            to_integer(Warclan::Ironsunz),
            to_integer(Warclan::Bloodtoofs),
            to_integer(Warclan::Da_Choppas)
    };

    constexpr std::array<int, 7> g_bossCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Hulking_Muscle_Bound_Brute),
            to_integer(CommandTrait::Live_to_Fight),
            to_integer(CommandTrait::Brutish_Cunning),
            to_integer(CommandTrait::Ironclad),
            to_integer(CommandTrait::Bestial_Charisma),
            to_integer(CommandTrait::Mighty_Waaagh)
    };

    constexpr std::array<int, 4> g_shamanCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Dead_Kunnin),
            to_integer(CommandTrait::Master_of_the_Weird),
            to_integer(CommandTrait::Bursting_with_Power)
    };

   constexpr std::array<int, 7> g_bossArtefacts = {
           to_integer(Artefact::None),
           to_integer(Artefact::Armour_of_Gork),
           to_integer(Artefact::Destroyer),
           to_integer(Artefact::Daubing_of_Mork),
           to_integer(Artefact::The_Golden_Toof),
           to_integer(Artefact::Metalrippas_Klaw),
           to_integer(Artefact::The_Boss_Skewer)
   };

    constexpr std::array<int, 4> g_shamanArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Great_Green_Visions),
            to_integer(Artefact::Amberbone_Hoard),
            to_integer(Artefact::Shamanic_Skullcape)
    };

    constexpr std::array<int, 7> g_mountTrait = {
        to_integer(MountTrait::None),
        to_integer(MountTrait::Big_Un),
        to_integer(MountTrait::Fast_Un),
        to_integer(MountTrait::Mean_Un),
        to_integer(MountTrait::Heavy_Un),
        to_integer(MountTrait::Loud_Un),
        to_integer(MountTrait::Weird_Un)
    };

    constexpr std::array<int, 4> g_warbeats = {
        to_integer(Warbeat::None),
        to_integer(Warbeat::Get_Em_Beat),
        to_integer(Warbeat::Fixin_Beat),
        to_integer(Warbeat::Killa_Beat)
    };

    constexpr std::array<int, 7> g_lore = {
        to_integer(Lore::None),
        to_integer(Lore::Brain_Bursta),
        to_integer(Lore::Mighty_Eadbutt),
        to_integer(Lore::Da_Blazin_Eyes),
        to_integer(Lore::Da_Great_Big_Green_Hand_of_Gork),
        to_integer(Lore::Bash_Em_Ladz),
        to_integer(Lore::Wrath_of_Gork)
    };

} // namespace Ironjawz

#endif // IRONJAWZPRIVATE_H