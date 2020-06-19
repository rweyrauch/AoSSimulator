/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FLESHEATERCOURTSPRIVATE_H
#define FLESHEATERCOURTSPRIVATE_H

#include <fec/FleshEaterCourts.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace FleshEaterCourt {

    constexpr std::array<int, 5> g_grandCourt = {
            to_integer(GrandCourt::None),
            to_integer(GrandCourt::Morgaunt),
            to_integer(GrandCourt::Hollowmourne),
            to_integer(GrandCourt::Blisterskin),
            to_integer(GrandCourt::Gristlegore)
    };

    constexpr std::array<int, 6> g_delusion = {
            to_integer(Delusion::Crusading_Army),
            to_integer(Delusion::The_Royal_Hunt),
            to_integer(Delusion::The_Feast_Day),
            to_integer(Delusion::A_Matter_of_Honour),
            to_integer(Delusion::The_Grand_Tournament),
            to_integer(Delusion::Defenders_of_the_Realm)
    };

    constexpr std::array<int, 7> g_abhorrantCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Bringer_of_Death),
            to_integer(CommandTrait::Frenzied_Flesheater),
            to_integer(CommandTrait::Savage_Beyond_Reason),
            to_integer(CommandTrait::Majestic_Horror),
            to_integer(CommandTrait::Dark_Wizardry),
            to_integer(CommandTrait::Completely_Delusional)
    };

    constexpr std::array<int, 7> g_courtierCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Bringer_of_Death),
            to_integer(CommandTrait::Frenzied_Flesheater),
            to_integer(CommandTrait::Savage_Beyond_Reason),
            to_integer(CommandTrait::Hulking_Brute),
            to_integer(CommandTrait::Cruel_Taskmaster),
            to_integer(CommandTrait::Dark_Acolyte)
    };

    constexpr std::array<int, 7> g_abhorrantArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Signet_of_the_First_Court),
            to_integer(Artefact::Splintervane_Brooch),
            to_integer(Artefact::Blood_River_Chalice),
            to_integer(Artefact::The_Grim_Garland),
            to_integer(Artefact::The_Dermal_Robe),
            to_integer(Artefact::Heart_of_the_Gargant)
    };

    constexpr std::array<int, 7> g_courtierArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Keening_Bone),
            to_integer(Artefact::Medal_of_Madness),
            to_integer(Artefact::The_Flayed_Pennant),
            to_integer(Artefact::Carrion_Wand),
            to_integer(Artefact::The_Fleshform_Raiment),
            to_integer(Artefact::The_Bilious_Decanter)
    };

    constexpr std::array<int, 7> g_lore = {
            to_integer(Lore::None),
            to_integer(Lore::Bonestorm),
            to_integer(Lore::Spectral_Host),
            to_integer(Lore::Monstrous_Vigour),
            to_integer(Lore::Miasmal_Shroud),
            to_integer(Lore::Deranged_Transformation),
            to_integer(Lore::Blood_Feast)
    };

    constexpr std::array<int, 7> g_terrorgheistMountTraits = {
            to_integer(MountTrait::None),
            to_integer(MountTrait::Deathly_Fast),
            to_integer(MountTrait::Razor_Clawed),
            to_integer(MountTrait::Horribly_Infested),
            to_integer(MountTrait::Horribly_Resilient),
            to_integer(MountTrait::Gruesome_Bite),
            to_integer(MountTrait::Devastating_Scream)
    };

    constexpr std::array<int, 7> g_dragonMountTraits = {
            to_integer(MountTrait::None),
            to_integer(MountTrait::Deathly_Fast),
            to_integer(MountTrait::Razor_Clawed),
            to_integer(MountTrait::Baneful_Breath),
            to_integer(MountTrait::Horribly_Resilient),
            to_integer(MountTrait::Necrotic_Fangs),
            to_integer(MountTrait::Death_From_the_Skies)
    };

} // namespace FleshEaterCourt

#endif // FLESHEATERCOURTSPRIVATE_H