/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SERAPHONPRIVATE_H
#define SERAPHONPRIVATE_H

#include <seraphon/Seraphon.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace Seraphon {

    const std::array<int, 2> g_wayOfTheSeraphon = {
            to_integer(WayOfTheSeraphon::Starborne),
            to_integer(WayOfTheSeraphon::Coalesced)
    };

    const std::array<int, 5> g_constellation = {
            to_integer(Constellation::None),
            to_integer(Constellation::Koatls_Claw),        // Coalesced
            to_integer(Constellation::Thunder_Lizard),     // Coalesced
            to_integer(Constellation::Dracothions_Tail),   // Starborne
            to_integer(Constellation::Fangs_of_Sotek),      // Starborne
    };

    const std::array<int, 4> g_slannCommandTrait = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Arcane_Might),
            to_integer(CommandTrait::Vast_Intellect),
            to_integer(CommandTrait::Great_Rememberer)
    };

    const std::array<int, 4> g_saurusCommandTrait {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Disciplined_Fury),
            to_integer(CommandTrait::Thickly_Scaled_Hide),
            to_integer(CommandTrait::Mighty_Warleader)
    };

    const std::array<int, 4> g_skinkCommandTrait {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Master_of_Star_Rituals),
            to_integer(CommandTrait::Nimble),
            to_integer(CommandTrait::Cunning)
    };

    const std::array<int, 7> g_treasuresOfTheOldOnes {
            to_integer(Artefact::None),
            to_integer(Artefact::Zoetic_Dial),
            to_integer(Artefact::Light_of_Dracothian),
            to_integer(Artefact::Prism_of_Amyntok),
            to_integer(Artefact::Itxi_Grubs),
            to_integer(Artefact::Plaque_of_Dominion),
            to_integer(Artefact::Throne_of_the_Lost_Gods)
    };

    const std::array<int, 4> g_celestialRelicsOfTheWarrior {
            to_integer(Artefact::None),
            to_integer(Artefact::Blade_of_Realities),
            to_integer(Artefact::Sigils_of_the_Prime_Hunter),
            to_integer(Artefact::Blood_Rage_Pendant)
    };

    const std::array<int, 4> g_vestmentsOfThePriesthood {
            to_integer(Artefact::None),
            to_integer(Artefact::Incandescent_Rectrices),
            to_integer(Artefact::Cloak_of_Feathers),
            to_integer(Artefact::Sacred_Stegadon_Helm)
    };

    const std::array<int, 7> g_loreOfCelestialDomination = {
            to_integer(Lore::None),
            to_integer(Lore::Celestial_Apotheosis),
            to_integer(Lore::Walk_Between_Realms),
            to_integer(Lore::Mystical_Unforging),
            to_integer(Lore::Celestial_Equilibrium),
            to_integer(Lore::Stellar_Tempest),
            to_integer(Lore::Drain_Magic)
    };

    const std::array<int, 7> g_loreOfCelestialManipulation = {
            to_integer(Lore::None),
            to_integer(Lore::Celestial_Harmony),
            to_integer(Lore::Hand_of_Glory),
            to_integer(Lore::Extend_Astromatrix),
            to_integer(Lore::Fiery_Convocation),
            to_integer(Lore::Bind_Endless_Spell),
            to_integer(Lore::Tide_of_Serpents)
    };

} // namespace Seraphon

#endif //SERAPHONPRIVATE_H