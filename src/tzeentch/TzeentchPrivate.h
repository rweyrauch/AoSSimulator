/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Tzeentch {

    constexpr std::array<int, 9> g_changeCoven = {
            ToInteger(ChangeCoven::None),
            ToInteger(ChangeCoven::Eternal_Conflagration),
            ToInteger(ChangeCoven::Hosts_Duplicitous),
            ToInteger(ChangeCoven::Hosts_Arcanum),
            ToInteger(ChangeCoven::Cult_Of_The_Transient_Form),
            ToInteger(ChangeCoven::Pyrofane_Cult),
            ToInteger(ChangeCoven::Guild_Of_Summoners),
            ToInteger(ChangeCoven::Unbound_Flux),
            ToInteger(ChangeCoven::Cult_Of_A_Thousand_Eyes)
    };

    constexpr std::array<int, 7> g_arcaniteCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Arch_Sorcerer),
            ToInteger(CommandTrait::Nexus_Of_Fate),
            ToInteger(CommandTrait::Magical_Supremancy),
            ToInteger(CommandTrait::Boundless_Mutation),
            ToInteger(CommandTrait::Cult_Demagogue),
            ToInteger(CommandTrait::Arcane_Sacrifice)
    };

    constexpr std::array<int, 4> g_mortalCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Nexus_Of_Fate),
            ToInteger(CommandTrait::Soul_Burn),
            ToInteger(CommandTrait::Illusionist)
    };

    constexpr std::array<int, 7> g_daemonCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Arch_Sorcerer),
            ToInteger(CommandTrait::Nexus_Of_Fate),
            ToInteger(CommandTrait::Magical_Supremancy),
            ToInteger(CommandTrait::Daemonspark),
            ToInteger(CommandTrait::Incorporeal_Form),
            ToInteger(CommandTrait::Aether_Tether)
    };

    constexpr std::array<int, 4> g_arcaniteArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Ambitions_End),
            ToInteger(Artefact::Secret_Eater),
            ToInteger(Artefact::Spiteful_Shield)
    };

    constexpr std::array<int, 7> g_mortalArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Wicked_Shard),
            ToInteger(Artefact::Changeblade),
            ToInteger(Artefact::Nexus_Staff),
            ToInteger(Artefact::Timeslip_Pendant),
            ToInteger(Artefact::Daemonheart),
            ToInteger(Artefact::Paradoxical_Shield)
    };

    constexpr std::array<int, 10> g_daemonArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Warpfire_Blade),
            ToInteger(Artefact::Sentient_Weapon),
            ToInteger(Artefact::Blade_Of_Fate),
            ToInteger(Artefact::Souleater),
            ToInteger(Artefact::Phatasmal_Weapons),
            ToInteger(Artefact::Pyrofyre_Stave),
            ToInteger(Artefact::Aura_Of_Mutabulity),
            ToInteger(Artefact::Wellspring_Of_Arcane_Might),
            ToInteger(Artefact::Aspect_Of_Tzeentch)
    };

    constexpr std::array<int, 7> g_loreOfFate = {
            ToInteger(Lore::None),
            ToInteger(Lore::Bolt_Of_Tzeentch),
            ToInteger(Lore::Arcane_Suggestion),
            ToInteger(Lore::Glimpse_The_Future),
            ToInteger(Lore::Shield_Of_Faith),
            ToInteger(Lore::Infusion_Arcanum),
            ToInteger(Lore::Treacherous_Bond)
    };

    constexpr std::array<int, 7> g_loreOfChange = {
            ToInteger(Lore::None),
            ToInteger(Lore::Bolt_Of_Tzeentch),
            ToInteger(Lore::Treason_Of_Tzeentch),
            ToInteger(Lore::Arcane_Transformation),
            ToInteger(Lore::Unchecked_Mutation),
            ToInteger(Lore::Fold_Reality),
            ToInteger(Lore::Tzeentchs_Firestorm)
    };

} // Tzeentch
