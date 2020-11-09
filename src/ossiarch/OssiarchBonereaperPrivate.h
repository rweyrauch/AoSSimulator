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
            ToInteger(Legion::None),
            ToInteger(Legion::Mortis_Praetorians),
            ToInteger(Legion::Petrifex_Elite),
            ToInteger(Legion::Stalliarch_Lords),
            ToInteger(Legion::Ivory_Host),
            ToInteger(Legion::Null_Myriad),
            ToInteger(Legion::Crematorians)
    };

    constexpr std::array<int, 7> g_kavalosCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Ancient_Knowledge),
            ToInteger(CommandTrait::Immortal_Ruler),
            ToInteger(CommandTrait::Dark_Acolyte),
            ToInteger(CommandTrait::Peerless_Warrior),
            ToInteger(CommandTrait::Hatred_Of_The_Living),
            ToInteger(CommandTrait::Life_Stealer)
    };

    constexpr std::array<int, 7> g_mortisanCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Ancient_Knowledge),
            ToInteger(CommandTrait::Immortal_Ruler),
            ToInteger(CommandTrait::Dire_Ultimatum),
            ToInteger(CommandTrait::Grave_Sand_Bones),
            ToInteger(CommandTrait::Oathbreaker_Curse),
            ToInteger(CommandTrait::Soul_Energy)
    };

    constexpr std::array<int, 7> g_kavaloiArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Mindblade),
            ToInteger(Artefact::Lordly_Phylactery),
            ToInteger(Artefact::Scroll_Of_Command),
            ToInteger(Artefact::Grave_Sand_Boneplate),
            ToInteger(Artefact::Marrowpact),
            ToInteger(Artefact::Helm_Of_The_Ordained)
    };

    constexpr std::array<int, 4> g_boneshaperArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Artisans_Key),
            ToInteger(Artefact::Lode_Of_Saturation),
            ToInteger(Artefact::The_Crafter_Gems)
    };

    constexpr std::array<int, 4> g_soulmasonArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Gothizzar_Cartouche),
            ToInteger(Artefact::Soul_Reservoir),
            ToInteger(Artefact::Throne_Of_Dzendt)
    };

    constexpr std::array<int, 4> g_soulreaperArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Luminscythe),
            ToInteger(Artefact::Vial_Of_Binding),
            ToInteger(Artefact::Guardian_Reavesoul)
    };

    constexpr std::array<int, 6> g_lore = {
            ToInteger(Lore::Arcane_Command),
            ToInteger(Lore::Empower_Nadirite_Weapons),
            ToInteger(Lore::Protection_Of_Nagash),
            ToInteger(Lore::Reinforce_Battle_Shields),
            ToInteger(Lore::Drain_Vitality),
            ToInteger(Lore::Mortal_Contract)
    };

} // namespace OssiarchBonereapers

#endif // OSSIARCHPRIVATE_H