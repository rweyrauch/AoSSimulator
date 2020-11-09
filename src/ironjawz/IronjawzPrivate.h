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
            ToInteger(Warclan::None),
            ToInteger(Warclan::Ironsunz),
            ToInteger(Warclan::Bloodtoofs),
            ToInteger(Warclan::Da_Choppas)
    };

    constexpr std::array<int, 7> g_bossCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Hulking_Muscle_Bound_Brute),
            ToInteger(CommandTrait::Live_To_Fight),
            ToInteger(CommandTrait::Brutish_Cunning),
            ToInteger(CommandTrait::Ironclad),
            ToInteger(CommandTrait::Bestial_Charisma),
            ToInteger(CommandTrait::Mighty_Waaagh)
    };

    constexpr std::array<int, 4> g_shamanCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Dead_Kunnin),
            ToInteger(CommandTrait::Master_Of_The_Weird),
            ToInteger(CommandTrait::Bursting_With_Power)
    };

   constexpr std::array<int, 7> g_bossArtefacts = {
           ToInteger(Artefact::None),
           ToInteger(Artefact::Armour_Of_Gork),
           ToInteger(Artefact::Destroyer),
           ToInteger(Artefact::Daubing_Of_Mork),
           ToInteger(Artefact::The_Golden_Toof),
           ToInteger(Artefact::Metalrippas_Klaw),
           ToInteger(Artefact::The_Boss_Skewer)
   };

    constexpr std::array<int, 4> g_shamanArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Great_Green_Visions),
            ToInteger(Artefact::Amberbone_Hoard),
            ToInteger(Artefact::Shamanic_Skullcape)
    };

    constexpr std::array<int, 7> g_mountTrait = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Big_Un),
            ToInteger(MountTrait::Fast_Un),
            ToInteger(MountTrait::Mean_Un),
            ToInteger(MountTrait::Heavy_Un),
            ToInteger(MountTrait::Loud_Un),
            ToInteger(MountTrait::Weird_Un)
    };

    constexpr std::array<int, 4> g_warbeats = {
            ToInteger(Warbeat::None),
            ToInteger(Warbeat::Get_Em_Beat),
            ToInteger(Warbeat::Fixin_Beat),
            ToInteger(Warbeat::Killa_Beat)
    };

    constexpr std::array<int, 7> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Brain_Bursta),
            ToInteger(Lore::Mighty_Eadbutt),
            ToInteger(Lore::Da_Blazin_Eyes),
            ToInteger(Lore::Da_Great_Big_Green_Hand_Of_Gork),
            ToInteger(Lore::Bash_Em_Ladz),
            ToInteger(Lore::Wrath_Of_Gork)
    };

} // namespace Ironjawz

#endif // IRONJAWZPRIVATE_H