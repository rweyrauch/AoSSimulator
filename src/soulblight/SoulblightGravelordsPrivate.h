/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <array>
#include "../../src/AoSSimPrivate.h"

namespace Soulblight {

    constexpr std::array<int, 6> g_bloodlines = {
            ToInteger(CursedBloodline::None),

            ToInteger(CursedBloodline::Legion_Of_Blood),
            ToInteger(CursedBloodline::Legion_Of_Night),
            ToInteger(CursedBloodline::Vyrkos_Dynasty),
            ToInteger(CursedBloodline::Kastelai_Dynasty),
            ToInteger(CursedBloodline::Avengorii_Dynasty)
    };

    constexpr std::array<int, 28> g_commandTraits = {
            ToInteger(CommandTrait::None),

            // Legion of Blood
            ToInteger(CommandTrait::Premeditated_Violence),
            ToInteger(CommandTrait::Soul_Crushing_Contempt),
            ToInteger(CommandTrait::Aristocracy_Of_Blood),
            ToInteger(CommandTrait::Aura_Of_Dark_Majesty),
            ToInteger(CommandTrait::Walking_Death),
            ToInteger(CommandTrait::Sanguine_Blur),

            // Legion of Night
            ToInteger(CommandTrait::Above_Suspicion),
            ToInteger(CommandTrait::Swift_Form),
            ToInteger(CommandTrait::Unbending_Will),
            ToInteger(CommandTrait::Merciless_Hunter),
            ToInteger(CommandTrait::Unholy_Impetus),
            ToInteger(CommandTrait::Terrifying_Visage),

            // Vyrkos Dynasty
            ToInteger(CommandTrait::Pack_Alpha),
            ToInteger(CommandTrait::Driven_By_Deathstench),
            ToInteger(CommandTrait::Kin_Of_The_Wolf),
            ToInteger(CommandTrait::Hunters_Snare),
            ToInteger(CommandTrait::Spoor_Trackers),
            ToInteger(CommandTrait::United_By_Blood),

            // Kastelai Dynasty
            ToInteger(CommandTrait::Beacon_Of_Bloodshed),
            ToInteger(CommandTrait::Master_Of_Retaliation),
            ToInteger(CommandTrait::Power_In_The_Blood),
            ToInteger(CommandTrait::Rousing_Commander),
            ToInteger(CommandTrait::Swift_And_Deadly),
            ToInteger(CommandTrait::A_Craving_For_Massacre),

            // Avengorii Dynasty
            ToInteger(CommandTrait::An_Eye_For_An_Eye),
            ToInteger(CommandTrait::Torment_Driven_Throes),
            ToInteger(CommandTrait::Unhinged_Rampager),
    };

    constexpr std::array<int, 28> g_artefacts = {
            ToInteger(Artefact::None),

            // Legion of Blood
            ToInteger(Artefact::Ring_Of_Domination),
            ToInteger(Artefact::Shadeglass_Decanter),
            ToInteger(Artefact::Orb_Of_Enchantment),
            ToInteger(Artefact::Soulbound_Garments),
            ToInteger(Artefact::Oubliette_Arcana),
            ToInteger(Artefact::Amulet_Of_Screams),

            // Legion of Night
            ToInteger(Artefact::Vial_Of_The_Pure_Blood),
            ToInteger(Artefact::Shard_Of_Night),
            ToInteger(Artefact::Gem_Of_Exsanguination),
            ToInteger(Artefact::Chiropteric_Cloak),
            ToInteger(Artefact::Morbhegs_Claw),
            ToInteger(Artefact::Curseblade),

            // Kastelai Dynasty
            ToInteger(Artefact::Sword_Of_The_Red_Seneschals),
            ToInteger(Artefact::Bloodsaints_Shield),
            ToInteger(Artefact::Stardard_Of_The_Crimson_Keep),
            ToInteger(Artefact::Grave_Sand_Shard),
            ToInteger(Artefact::Fragment_Of_The_Keep),
            ToInteger(Artefact::The_Red_Casket),

            // Vyrkos Dynaster
            ToInteger(Artefact::Ulfenkarnian_Phylactery),
            ToInteger(Artefact::Cloak_Of_The_Night_Prowler),
            ToInteger(Artefact::Sangsyron),
            ToInteger(Artefact::Vilnas_Fang),
            ToInteger(Artefact::Terminus_Clock),
            ToInteger(Artefact::Standard_Of_The_Ulfenwatch),

            // Avengorii Dynasty
            ToInteger(Artefact::Breath_Of_The_Void_Maw),
            ToInteger(Artefact::Ghorvars_Collar),
            ToInteger(Artefact::The_Furious_Crown)
    };

    constexpr std::array<int, 7> g_deathmageLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Overwhelming_Dread),
            ToInteger(Lore::Fading_Vigour),
            ToInteger(Lore::Spectral_Grasp),
            ToInteger(Lore::Prison_Of_Grief),
            ToInteger(Lore::Decrepify),
            ToInteger(Lore::Soul_Harvest),
    };

    constexpr std::array<int, 7> g_vampireLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Blades_Of_Shyish),
            ToInteger(Lore::Spirit_Gale),
            ToInteger(Lore::Vile_Transference),
            ToInteger(Lore::Amethystine_Pinions),
            ToInteger(Lore::Soulpike),
            ToInteger(Lore::Amaranthine_Orb),
    };

    constexpr std::array<int, 4> g_mutations = {
            ToInteger(Mutation::None),
            ToInteger(Mutation::Maddening_Hunger),
            ToInteger(Mutation::Urges_Of_Atrocity),
            ToInteger(Mutation::Nullblood_Construct)
    };

} // namespace Soulblight
