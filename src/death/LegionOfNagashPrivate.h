/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LEGIONOFNAGASHPRIVATE_H
#define LEGIONOFNAGASHPRIVATE_H

#include <death/LegionOfNagash.h>
#include <array>
#include "../../src/AoSSimPrivate.h"

namespace Death {

    constexpr std::array<int, 4> g_legions = {
            to_integer(Legion::Grand_Host_of_Nagash),
            to_integer(Legion::Legion_of_Sacrament),
            to_integer(Legion::Legion_of_Blood),
            to_integer(Legion::Legion_of_Night)
    };

    constexpr std::array<int, 37> g_commandTraits = {
            to_integer(CommandTrait::None),

            // Grand Host
            to_integer(CommandTrait::Master_of_Death),
            to_integer(CommandTrait::Chosen_Champion),
            to_integer(CommandTrait::Bane_of_the_Living),
            to_integer(CommandTrait::Aura_of_Ages),
            to_integer(CommandTrait::Ancient_Strategist),
            to_integer(CommandTrait::Lord_of_Nagashizzar),

            // Legion of Sacrament
            to_integer(CommandTrait::Emissary_of_the_Master),
            to_integer(CommandTrait::Mark_of_the_Favoured),
            to_integer(CommandTrait::Dark_Acolyte),
            to_integer(CommandTrait::Mastery_of_Death),
            to_integer(CommandTrait::Peerless_Commander),
            to_integer(CommandTrait::Bound_to_the_Master),

            // Legion of Blood
            to_integer(CommandTrait::Swift_Strikes),
            to_integer(CommandTrait::Soul_Crushing_Contempt),
            to_integer(CommandTrait::Aristocracy_of_Blood),
            to_integer(CommandTrait::Aura_of_Dark_Majesty),
            to_integer(CommandTrait::Walking_Death),
            to_integer(CommandTrait::Sanguine_Blur),

            // Legion of Night
            to_integer(CommandTrait::Above_Suspicion),
            to_integer(CommandTrait::Swift_Form),
            to_integer(CommandTrait::Unbending_Will),
            to_integer(CommandTrait::Merciless_Hunter),
            to_integer(CommandTrait::Unholy_Impetus),
            to_integer(CommandTrait::Terrifying_Visage),

            // Soulblight
            to_integer(CommandTrait::Curse_of_the_Revenant),
            to_integer(CommandTrait::Deathless_Duellist),
            to_integer(CommandTrait::Transfix),
            to_integer(CommandTrait::Mist_Form),
            to_integer(CommandTrait::Killing_Blow),
            to_integer(CommandTrait::Blood_Fury),

            // Death
            to_integer(CommandTrait::Ruler_of_the_Night),
            to_integer(CommandTrait::Predator_of_the_Shadows),
            to_integer(CommandTrait::Death_Incarnate),
            to_integer(CommandTrait::Master_of_the_Black_Arts),
            to_integer(CommandTrait::Red_Fury),
            to_integer(CommandTrait::Supernatural_Horror),
    };

    constexpr std::array<int, 25> g_artefacts = {
            to_integer(Artefact::None),

            // Grand Host
            to_integer(Artefact::Deathforged_Chain),
            to_integer(Artefact::Balefire_Lantern),
            to_integer(Artefact::Grave_Sand_Timeglass),
            to_integer(Artefact::Ossific_Diadem),
            to_integer(Artefact::Amethyst_Shard),
            to_integer(Artefact::Terrorgheist_Mantle),

            // Legion of Sacrament
            to_integer(Artefact::Spiritcage),
            to_integer(Artefact::Shroud_of_Darkness),
            to_integer(Artefact::Asylumaticae),
            to_integer(Artefact::Wristbands_of_Black_Gold),
            to_integer(Artefact::Azyrbane_Standard),
            to_integer(Artefact::Black_Gem),

            // Legion of Blood
            to_integer(Artefact::Ring_of_Domination),
            to_integer(Artefact::Shadeglass_Decanter),
            to_integer(Artefact::Orb_of_Enchantment),
            to_integer(Artefact::Soulbound_Garments),
            to_integer(Artefact::Oubliette_Arcana),
            to_integer(Artefact::Amulet_of_Screams),

            // Legion of Night
            to_integer(Artefact::Vial_of_the_Pure_Blood),
            to_integer(Artefact::Shard_of_Night),
            to_integer(Artefact::Gem_of_Exsanguination),
            to_integer(Artefact::Chiropteric_Cloak),
            to_integer(Artefact::Morbhegs_Claw),
            to_integer(Artefact::Curseblade),
    };

    constexpr std::array<int, 13> g_deathmageLore = {
            to_integer(Lore::None),
            to_integer(Lore::Overwhelming_Dread),
            to_integer(Lore::Fading_Vigour),
            to_integer(Lore::Spectral_Grasp),
            to_integer(Lore::Prison_of_Grief),
            to_integer(Lore::Decrepify),
            to_integer(Lore::Soul_Harvest),
    };

    constexpr std::array<int, 13> g_vampireLore = {
            to_integer(Lore::None),
            to_integer(Lore::Blades_of_Shyish),
            to_integer(Lore::Spirit_Gale),
            to_integer(Lore::Vile_Transference),
            to_integer(Lore::Amethystine_Pinions),
            to_integer(Lore::Soulpike),
            to_integer(Lore::Amaranthine_Orb),
    };

} // namespace Death

#endif //LEGIONOFNAGASHPRIVATE_H