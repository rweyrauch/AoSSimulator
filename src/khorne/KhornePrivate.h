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
            ToInteger(SlaughterHost::None),
            ToInteger(SlaughterHost::Reapers_Of_Vengeance),
            ToInteger(SlaughterHost::Bloodlords),
            ToInteger(SlaughterHost::Goretide),
            ToInteger(SlaughterHost::Skullfiend_Tribe)
    };

    constexpr std::array<int, 7> g_mortalbloodboundCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Arch_Slaughterer),
            ToInteger(CommandTrait::Unrivalled_Battle_Lust),
            ToInteger(CommandTrait::Slaughterborn),
            ToInteger(CommandTrait::Hungry_For_Glory),
            ToInteger(CommandTrait::Berserker_Lord),
            ToInteger(CommandTrait::Violent_Urgency)
    };

    constexpr std::array<int, 7> g_bloodboundCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Arch_Slaughterer),
            ToInteger(CommandTrait::Unrivalled_Battle_Lust),
            ToInteger(CommandTrait::Slaughterborn),
            ToInteger(CommandTrait::Mark_Of_The_Cannibal),
            ToInteger(CommandTrait::Bloodsworn),
            ToInteger(CommandTrait::Disciple_Of_Khorne),
    };

    constexpr std::array<int, 7> g_daemonCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Arch_Slaughterer),
            ToInteger(CommandTrait::Unrivalled_Battle_Lust),
            ToInteger(CommandTrait::Slaughterborn),
            ToInteger(CommandTrait::Rage_Unchained),
            ToInteger(CommandTrait::Aspect_Of_Death),
            ToInteger(CommandTrait::Devastating_Blow)
    };

    // Mortal Bloodbound
    constexpr std::array<int, 13> g_mortalArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Heart_Seeker),
            ToInteger(Artefact::Collar_Of_Contempt),
            ToInteger(Artefact::Blood_Drinker),
            ToInteger(Artefact::Gorecleaver),
            ToInteger(Artefact::The_Crimson_Plate),
            ToInteger(Artefact::Blood_Rune),
            ToInteger(Artefact::Skull_Helm_Of_Khorne),
            ToInteger(Artefact::Blood_Forged_Armour),
            ToInteger(Artefact::Brazen_Rune),
            ToInteger(Artefact::Blade_Of_Endless_Bloodshed),
            ToInteger(Artefact::Mask_Of_The_Destroyer),
            ToInteger(Artefact::Talisman_Of_Burning_Blood)
    };

    // Khorne Bloodbound with Totem
    constexpr std::array<int, 16> g_totemArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Heart_Seeker),
            ToInteger(Artefact::Collar_Of_Contempt),
            ToInteger(Artefact::Blood_Drinker),
            ToInteger(Artefact::Gorecleaver),
            ToInteger(Artefact::The_Crimson_Plate),
            ToInteger(Artefact::Blood_Rune),
            ToInteger(Artefact::Skull_Helm_Of_Khorne),
            ToInteger(Artefact::Blood_Forged_Armour),
            ToInteger(Artefact::Brazen_Rune),
            ToInteger(Artefact::Blade_Of_Endless_Bloodshed),
            ToInteger(Artefact::Mask_Of_The_Destroyer),
            ToInteger(Artefact::Talisman_Of_Burning_Blood),
            ToInteger(Artefact::Banner_Of_Rage),
            ToInteger(Artefact::Banner_Of_Wrath),
            ToInteger(Artefact::Banner_Of_Blood)
    };

    // Khorne Daemons
    constexpr std::array<int, 13> g_daemonArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Argath_The_King_Of_Blades),
            ToInteger(Artefact::Deathdealer),
            ToInteger(Artefact::Khartoth_The_Bloodhunger),
            ToInteger(Artefact::Hellfire_Blade),
            ToInteger(Artefact::Harvester_Of_Skulls),
            ToInteger(Artefact::Foes_Bane),
            ToInteger(Artefact::The_Crimson_Crown),
            ToInteger(Artefact::Armour_Of_Scorn),
            ToInteger(Artefact::Mark_Of_The_Bloodreaper),
            ToInteger(Artefact::Collar_Of_Khorne),
            ToInteger(Artefact::Crimson_Soulstone),
            ToInteger(Artefact::Mark_Of_The_Slayer)
    };

    // Prayers
    constexpr std::array<int, 6> g_bloodBlessingsOfKhorne = {
            ToInteger(BloodBlessingsOfKhorne::Bronzed_Flesh),
            ToInteger(BloodBlessingsOfKhorne::Blood_Sacrifice),
            ToInteger(BloodBlessingsOfKhorne::Resanguination),
            ToInteger(BloodBlessingsOfKhorne::Brazen_Fury),
            ToInteger(BloodBlessingsOfKhorne::Killing_Frenzy),
            ToInteger(BloodBlessingsOfKhorne::Spellbane_Hex)
    };

    // Blood Tithe Rewards
    constexpr std::array<int, 9> g_bloodTitheRewards = {
            ToInteger(BloodTitheReward::None),
            ToInteger(BloodTitheReward::Bloody_Examplar),
            ToInteger(BloodTitheReward::Spelleater_Curse),
            ToInteger(BloodTitheReward::Murderlust),
            ToInteger(BloodTitheReward::Apoplectic_Frenzy),
            ToInteger(BloodTitheReward::Brass_Skull_Meteor),
            ToInteger(BloodTitheReward::Relentless_Fury),
            ToInteger(BloodTitheReward::Crimson_Rain)
    };

} // namespace Khorne

#endif // KHORNEPRIVATE_H