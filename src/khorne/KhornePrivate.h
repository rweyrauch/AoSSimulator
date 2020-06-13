/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHORNEPRIVATE_H
#define KHORNEPRIVATE_H

#include <khorne/KhorneBase.h>
#include <khorne/BloodBlessingsOfKhorne.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Khorne {

    constexpr std::array<int, 5> g_slaughterHost = {
            to_integer(SlaughterHost::None),
            to_integer(SlaughterHost::Reapers_of_Vengeance),
            to_integer(SlaughterHost::Bloodlords),
            to_integer(SlaughterHost::Goretide),
            to_integer(SlaughterHost::Skullfiend_Tribe)
    };

    constexpr std::array<int, 7> g_mortalbloodboundCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Arch_Slaughterer),
            to_integer(CommandTrait::Unrivalled_Battle_Lust),
            to_integer(CommandTrait::Slaughterborn),
            to_integer(CommandTrait::Hungry_for_Glory),
            to_integer(CommandTrait::Berserker_Lord),
            to_integer(CommandTrait::Violent_Urgency)
    };

    constexpr std::array<int, 7> g_bloodboundCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Arch_Slaughterer),
            to_integer(CommandTrait::Unrivalled_Battle_Lust),
            to_integer(CommandTrait::Slaughterborn),
            to_integer(CommandTrait::Mark_of_the_Cannibal),
            to_integer(CommandTrait::Bloodsworn),
            to_integer(CommandTrait::Disciple_of_Khorne),
    };

    constexpr std::array<int, 7> g_daemonCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Arch_Slaughterer),
            to_integer(CommandTrait::Unrivalled_Battle_Lust),
            to_integer(CommandTrait::Slaughterborn),
            to_integer(CommandTrait::Rage_Unchained),
            to_integer(CommandTrait::Aspect_of_Death),
            to_integer(CommandTrait::Devastating_Blow)
    };

    // Mortal Bloodbound
    constexpr std::array<int, 13> g_mortalArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Heart_Seeker),
            to_integer(Artefact::Collar_of_Contempt),
            to_integer(Artefact::Blood_Drinker),
            to_integer(Artefact::Gorecleaver),
            to_integer(Artefact::The_Crimson_Plate),
            to_integer(Artefact::Blood_Rune),
            to_integer(Artefact::Skull_Helm_of_Khorne),
            to_integer(Artefact::Blood_Forged_Armour),
            to_integer(Artefact::Brazen_Rune),
            to_integer(Artefact::Blade_of_Endless_Bloodshed),
            to_integer(Artefact::Mask_of_the_Destroyer),
            to_integer(Artefact::Talisman_of_Burning_Blood)
    };

    // Khorne Bloodbound with Totem
    constexpr std::array<int, 16> g_totemArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Heart_Seeker),
            to_integer(Artefact::Collar_of_Contempt),
            to_integer(Artefact::Blood_Drinker),
            to_integer(Artefact::Gorecleaver),
            to_integer(Artefact::The_Crimson_Plate),
            to_integer(Artefact::Blood_Rune),
            to_integer(Artefact::Skull_Helm_of_Khorne),
            to_integer(Artefact::Blood_Forged_Armour),
            to_integer(Artefact::Brazen_Rune),
            to_integer(Artefact::Blade_of_Endless_Bloodshed),
            to_integer(Artefact::Mask_of_the_Destroyer),
            to_integer(Artefact::Talisman_of_Burning_Blood),
            to_integer(Artefact::Banner_of_Rage),
            to_integer(Artefact::Banner_of_Wrath),
            to_integer(Artefact::Banner_of_Blood)
    };

    // Khorne Daemons
    constexpr std::array<int, 13> g_daemonArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Argath_the_King_of_Blades),
            to_integer(Artefact::Deathdealer),
            to_integer(Artefact::Khartoth_the_Bloodhunger),
            to_integer(Artefact::Hellfire_Blade),
            to_integer(Artefact::Harvester_of_Skulls),
            to_integer(Artefact::Foes_Bane),
            to_integer(Artefact::The_Crimson_Crown),
            to_integer(Artefact::Armour_of_Scorn),
            to_integer(Artefact::Mark_of_the_Bloodreaper),
            to_integer(Artefact::Collar_of_Khorne),
            to_integer(Artefact::Crimson_Soulstone),
            to_integer(Artefact::Mark_of_the_Slayer)
    };

    // Prayers
    constexpr std::array<int, 6> g_bloodBlessingsOfKhorne = {
            to_integer(BloodBlessingsOfKhorne::Bronzed_Flesh),
            to_integer(BloodBlessingsOfKhorne::Blood_Sacrifice),
            to_integer(BloodBlessingsOfKhorne::Resanguination),
            to_integer(BloodBlessingsOfKhorne::Brazen_Fury),
            to_integer(BloodBlessingsOfKhorne::Killing_Frenzy),
            to_integer(BloodBlessingsOfKhorne::Spellbane_Hex)
    };

} // namespace Khorne

#endif // KHORNEPRIVATE_H