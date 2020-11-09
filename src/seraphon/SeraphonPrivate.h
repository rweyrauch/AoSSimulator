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
            ToInteger(WayOfTheSeraphon::Starborne),
            ToInteger(WayOfTheSeraphon::Coalesced)
    };

    const std::array<int, 5> g_constellation = {
            ToInteger(Constellation::None),
            ToInteger(Constellation::Koatls_Claw),        // Coalesced
            ToInteger(Constellation::Thunder_Lizard),     // Coalesced
            ToInteger(Constellation::Dracothions_Tail),   // Starborne
            ToInteger(Constellation::Fangs_Of_Sotek),      // Starborne
    };

    const std::array<int, 4> g_slannCommandTrait = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Arcane_Might),
            ToInteger(CommandTrait::Vast_Intellect),
            ToInteger(CommandTrait::Great_Rememberer)
    };

    const std::array<int, 4> g_saurusCommandTrait {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Disciplined_Fury),
            ToInteger(CommandTrait::Thickly_Scaled_Hide),
            ToInteger(CommandTrait::Mighty_Warleader)
    };

    const std::array<int, 4> g_skinkCommandTrait {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Master_Of_Star_Rituals),
            ToInteger(CommandTrait::Nimble),
            ToInteger(CommandTrait::Cunning)
    };

    const std::array<int, 7> g_treasuresOfTheOldOnes {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Zoetic_Dial),
            ToInteger(Artefact::Light_Of_Dracothian),
            ToInteger(Artefact::Prism_Of_Amyntok),
            ToInteger(Artefact::Itxi_Grubs),
            ToInteger(Artefact::Plaque_Of_Dominion),
            ToInteger(Artefact::Throne_Of_The_Lost_Gods)
    };

    const std::array<int, 4> g_celestialRelicsOfTheWarrior {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Blade_Of_Realities),
            ToInteger(Artefact::Sigils_Of_The_Prime_Hunter),
            ToInteger(Artefact::Blood_Rage_Pendant)
    };

    const std::array<int, 4> g_vestmentsOfThePriesthood {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Incandescent_Rectrices),
            ToInteger(Artefact::Cloak_Of_Feathers),
            ToInteger(Artefact::Sacred_Stegadon_Helm)
    };

    const std::array<int, 7> g_loreOfCelestialDomination = {
            ToInteger(Lore::None),
            ToInteger(Lore::Celestial_Apotheosis),
            ToInteger(Lore::Walk_Between_Realms),
            ToInteger(Lore::Mystical_Unforging),
            ToInteger(Lore::Celestial_Equilibrium),
            ToInteger(Lore::Stellar_Tempest),
            ToInteger(Lore::Drain_Magic)
    };

    const std::array<int, 7> g_loreOfCelestialManipulation = {
            ToInteger(Lore::None),
            ToInteger(Lore::Celestial_Harmony),
            ToInteger(Lore::Hand_Of_Glory),
            ToInteger(Lore::Extend_Astromatrix),
            ToInteger(Lore::Fiery_Convocation),
            ToInteger(Lore::Bind_Endless_Spell),
            ToInteger(Lore::Tide_Of_Serpents)
    };

} // namespace Seraphon

#endif //SERAPHONPRIVATE_H