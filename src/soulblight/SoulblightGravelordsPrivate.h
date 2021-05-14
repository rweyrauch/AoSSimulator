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

    constexpr std::array<int, 6> g_legions = {
            ToInteger(CursedBloodline::None),

            ToInteger(CursedBloodline::Legion_Of_Blood),
            ToInteger(CursedBloodline::Legion_Of_Night),
            ToInteger(CursedBloodline::Vyrkos_Dynasty),
            ToInteger(CursedBloodline::Kastelai_Dynasty),
            ToInteger(CursedBloodline::Avengorii_Dynasty)
    };

    constexpr std::array<int, 37> g_commandTraits = {
            ToInteger(CommandTrait::None),

            // Grand Host
            ToInteger(CommandTrait::Master_Of_Death),
            ToInteger(CommandTrait::Chosen_Champion),
            ToInteger(CommandTrait::Bane_Of_The_Living),
            ToInteger(CommandTrait::Aura_Of_Ages),
            ToInteger(CommandTrait::Ancient_Strategist),
            ToInteger(CommandTrait::Lord_Of_Nagashizzar),

            // Legion of Sacrament
            ToInteger(CommandTrait::Emissary_Of_The_Master),
            ToInteger(CommandTrait::Mark_Of_The_Favoured),
            ToInteger(CommandTrait::Dark_Acolyte),
            ToInteger(CommandTrait::Mastery_Of_Death),
            ToInteger(CommandTrait::Peerless_Commander),
            ToInteger(CommandTrait::Bound_To_The_Master),

            // Legion of Blood
            ToInteger(CommandTrait::Swift_Strikes),
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

            // Soulblight
            ToInteger(CommandTrait::Curse_Of_The_Revenant),
            ToInteger(CommandTrait::Deathless_Duellist),
            ToInteger(CommandTrait::Transfix),
            ToInteger(CommandTrait::Mist_Form),
            ToInteger(CommandTrait::Killing_Blow),
            ToInteger(CommandTrait::Blood_Fury),

            // Death
            ToInteger(CommandTrait::Ruler_Of_The_Night),
            ToInteger(CommandTrait::Predator_Of_The_Shadows),
            ToInteger(CommandTrait::Death_Incarnate),
            ToInteger(CommandTrait::Master_Of_The_Black_Arts),
            ToInteger(CommandTrait::Red_Fury),
            ToInteger(CommandTrait::Supernatural_Horror),
    };

    constexpr std::array<int, 25> g_artefacts = {
            ToInteger(Artefact::None),

            // Grand Host
            ToInteger(Artefact::Deathforged_Chain),
            ToInteger(Artefact::Balefire_Lantern),
            ToInteger(Artefact::Grave_Sand_Timeglass),
            ToInteger(Artefact::Ossific_Diadem),
            ToInteger(Artefact::Amethyst_Shard),
            ToInteger(Artefact::Terrorgheist_Mantle),

            // Legion of Sacrament
            ToInteger(Artefact::Spiritcage),
            ToInteger(Artefact::Shroud_Of_Darkness),
            ToInteger(Artefact::Asylumaticae),
            ToInteger(Artefact::Wristbands_Of_Black_Gold),
            ToInteger(Artefact::Azyrbane_Standard),
            ToInteger(Artefact::Black_Gem),

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

} // namespace Soulblight
