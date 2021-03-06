/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fec/FleshEaterCourts.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace FleshEaterCourt {

    constexpr std::array<int, 5> g_grandCourt = {
            ToInteger(GrandCourt::None),
            ToInteger(GrandCourt::Morgaunt),
            ToInteger(GrandCourt::Hollowmourne),
            ToInteger(GrandCourt::Blisterskin),
            ToInteger(GrandCourt::Gristlegore)
    };

    constexpr std::array<int, 6> g_delusion = {
            ToInteger(Delusion::Crusading_Army),
            ToInteger(Delusion::The_Royal_Hunt),
            ToInteger(Delusion::The_Feast_Day),
            ToInteger(Delusion::A_Matter_Of_Honour),
            ToInteger(Delusion::The_Grand_Tournament),
            ToInteger(Delusion::Defenders_Of_The_Realm)
    };

    constexpr std::array<int, 7> g_abhorrantCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Bringer_Of_Death),
            ToInteger(CommandTrait::Frenzied_Flesheater),
            ToInteger(CommandTrait::Savage_Beyond_Reason),
            ToInteger(CommandTrait::Majestic_Horror),
            ToInteger(CommandTrait::Dark_Wizardry),
            ToInteger(CommandTrait::Completely_Delusional)
    };

    constexpr std::array<int, 7> g_courtierCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Bringer_Of_Death),
            ToInteger(CommandTrait::Frenzied_Flesheater),
            ToInteger(CommandTrait::Savage_Beyond_Reason),
            ToInteger(CommandTrait::Hulking_Brute),
            ToInteger(CommandTrait::Cruel_Taskmaster),
            ToInteger(CommandTrait::Dark_Acolyte)
    };

    constexpr std::array<int, 7> g_abhorrantArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Signet_Of_The_First_Court),
            ToInteger(Artefact::Splintervane_Brooch),
            ToInteger(Artefact::Blood_River_Chalice),
            ToInteger(Artefact::The_Grim_Garland),
            ToInteger(Artefact::The_Dermal_Robe),
            ToInteger(Artefact::Heart_Of_The_Gargant)
    };

    constexpr std::array<int, 7> g_courtierArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Keening_Bone),
            ToInteger(Artefact::Medal_Of_Madness),
            ToInteger(Artefact::The_Flayed_Pennant),
            ToInteger(Artefact::Carrion_Wand),
            ToInteger(Artefact::The_Fleshform_Raiment),
            ToInteger(Artefact::The_Bilious_Decanter)
    };

    constexpr std::array<int, 7> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Bonestorm),
            ToInteger(Lore::Spectral_Host),
            ToInteger(Lore::Monstrous_Vigour),
            ToInteger(Lore::Miasmal_Shroud),
            ToInteger(Lore::Deranged_Transformation),
            ToInteger(Lore::Blood_Feast)
    };

    constexpr std::array<int, 7> g_terrorgheistMountTraits = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Deathly_Fast),
            ToInteger(MountTrait::Razor_Clawed),
            ToInteger(MountTrait::Horribly_Infested),
            ToInteger(MountTrait::Horribly_Resilient),
            ToInteger(MountTrait::Gruesome_Bite),
            ToInteger(MountTrait::Devastating_Scream)
    };

    constexpr std::array<int, 7> g_dragonMountTraits = {
            ToInteger(MountTrait::None),
            ToInteger(MountTrait::Deathly_Fast),
            ToInteger(MountTrait::Razor_Clawed),
            ToInteger(MountTrait::Baneful_Breath),
            ToInteger(MountTrait::Horribly_Resilient),
            ToInteger(MountTrait::Necrotic_Fangs),
            ToInteger(MountTrait::Death_From_The_Skies)
    };

} // namespace FleshEaterCourt
