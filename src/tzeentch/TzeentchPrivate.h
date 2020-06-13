/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZEENTCHPRIVATE_H
#define TZEENTCHPRIVATE_H

#include <tzeentch/TzeentchBase.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Tzeentch {

    constexpr std::array<int, 7> g_changeCoven = {
            to_integer(ChangeCoven::None),
            to_integer(ChangeCoven::EternalConflagration),
            to_integer(ChangeCoven::HostsDuplicitous),
            to_integer(ChangeCoven::HostsArcanum),
            to_integer(ChangeCoven::CultOfTheTransientForm),
            to_integer(ChangeCoven::PyrofaneCult),
            to_integer(ChangeCoven::GuildOfSummoners)
    };

    constexpr std::array<int, 7> g_arcaniteCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Arch_Sorcerer),
            to_integer(CommandTrait::Nexus_of_Fate),
            to_integer(CommandTrait::Magical_Supremancy),
            to_integer(CommandTrait::Boundless_Mutation),
            to_integer(CommandTrait::Cult_Demagogue),
            to_integer(CommandTrait::Arcane_Sacrifice)
    };

    constexpr std::array<int, 4> g_mortalCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Nexus_of_Fate),
            to_integer(CommandTrait::Soul_Burn),
            to_integer(CommandTrait::Illusionist)
    };

    constexpr std::array<int, 7> g_daemonCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Arch_Sorcerer),
            to_integer(CommandTrait::Nexus_of_Fate),
            to_integer(CommandTrait::Magical_Supremancy),
            to_integer(CommandTrait::Daemonspark),
            to_integer(CommandTrait::Incorporeal_Form),
            to_integer(CommandTrait::Aether_Tether)
    };

    constexpr std::array<int, 4> g_arcaniteArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Ambitions_End),
            to_integer(Artefact::Secret_Eater),
            to_integer(Artefact::Spiteful_Shield)
    };

    constexpr std::array<int, 7> g_mortalArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Wicked_Shard),
            to_integer(Artefact::Changeblade),
            to_integer(Artefact::Nexus_Staff),
            to_integer(Artefact::Timeslip_Pendant),
            to_integer(Artefact::Daemonheart),
            to_integer(Artefact::Paradoxical_Shield)
    };

    constexpr std::array<int, 10> g_daemonArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Warpfire_Blade),
            to_integer(Artefact::Sentient_Weapon),
            to_integer(Artefact::Blade_of_Fate),
            to_integer(Artefact::Souleater),
            to_integer(Artefact::Phatasmal_Weapons),
            to_integer(Artefact::Pyrofyre_Stave),
            to_integer(Artefact::Aura_of_Mutabulity),
            to_integer(Artefact::Wellspring_of_Arcane_Might),
            to_integer(Artefact::Aspect_of_Tzeentch)
    };

    constexpr std::array<int, 6> g_loreOfFate = {
            to_integer(Lore::Bolt_of_Tzeentch),
            to_integer(Lore::Arcane_Suggestion),
            to_integer(Lore::Glimpse_the_Future),
            to_integer(Lore::Shield_of_Faith),
            to_integer(Lore::Infusion_Arcanum),
            to_integer(Lore::Treacherous_Bond)
    };

    constexpr std::array<int, 6> g_loreOfChange = {
            to_integer(Lore::Bolt_of_Tzeentch),
            to_integer(Lore::Treason_of_Tzeentch),
            to_integer(Lore::Arcane_Transformation),
            to_integer(Lore::Unchecked_Mutation),
            to_integer(Lore::Fold_Reality),
            to_integer(Lore::Tzeentchs_Firestorm)
    };

} // Tzeentch

#endif //TZEENTCHPRIVATE_H