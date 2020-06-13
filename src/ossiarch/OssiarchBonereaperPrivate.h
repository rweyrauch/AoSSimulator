/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef OSSIARCHPRIVATE_H
#define OSSIARCHPRIVATE_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace OssiarchBonereapers {

    constexpr std::array<int, 7> g_legion = {
            to_integer(Legion::None),
            to_integer(Legion::Mortis_Praetorians),
            to_integer(Legion::Petrifex_Elite),
            to_integer(Legion::Stalliarch_Lords),
            to_integer(Legion::Ivory_Host),
            to_integer(Legion::Null_Myriad),
            to_integer(Legion::Crematorians)
    };

    constexpr std::array<int, 7> g_kavalosCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Ancient_Knowledge),
            to_integer(CommandTrait::Immortal_Ruler),
            to_integer(CommandTrait::Dark_Acolyte),
            to_integer(CommandTrait::Peerless_Warrior),
            to_integer(CommandTrait::Hatred_of_the_Living),
            to_integer(CommandTrait::Life_Stealer)
    };

    constexpr std::array<int, 7> g_mortisanCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Ancient_Knowledge),
            to_integer(CommandTrait::Immortal_Ruler),
            to_integer(CommandTrait::Dire_Ultimatum),
            to_integer(CommandTrait::Grave_Sand_Bones),
            to_integer(CommandTrait::Oathbreaker_Curse),
            to_integer(CommandTrait::Soul_Energy)
    };

    constexpr std::array<int, 7> g_kavaloiArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Mindblade),
            to_integer(Artefact::Lordly_Phylactery),
            to_integer(Artefact::Scroll_of_Command),
            to_integer(Artefact::Grave_Sand_Boneplate),
            to_integer(Artefact::Marrowpact),
            to_integer(Artefact::Helm_of_the_Ordained)
    };

    constexpr std::array<int, 4> g_boneshaperArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Artisans_Key),
            to_integer(Artefact::Lode_of_Saturation),
            to_integer(Artefact::The_Crafter_Gems)
    };

    constexpr std::array<int, 4> g_soulmasonArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Gothizzar_Cartouche),
            to_integer(Artefact::Soul_Reservoir),
            to_integer(Artefact::Throne_of_Dzendt)
    };

    constexpr std::array<int, 4> g_soulreaperArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Luminscythe),
            to_integer(Artefact::Vial_of_Binding),
            to_integer(Artefact::Guardian_Reavesoul)
    };

    constexpr std::array<int, 6> g_lore = {
            to_integer(Lore::Arcane_Command),
            to_integer(Lore::Empower_Nadirite_Weapons),
            to_integer(Lore::Protection_of_Nagash),
            to_integer(Lore::Reinforce_Battle_Shields),
            to_integer(Lore::Drain_Vitality),
            to_integer(Lore::Mortal_Contract)
    };

} // namespace OssiarchBonereapers

#endif // OSSIARCHPRIVATE_H