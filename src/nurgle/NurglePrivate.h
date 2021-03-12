/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nurgle/Nurgle.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Nurgle {

    constexpr std::array<int, 5> g_plagueLegions = {
            ToInteger(PlagueLegion::None),
            ToInteger(PlagueLegion::Munificent_Wanderers),
            ToInteger(PlagueLegion::Drowning_Guard),
            ToInteger(PlagueLegion::Blessed_Sons),
            ToInteger(PlagueLegion::Drowned_Men)
    };

    constexpr std::array<int, 19> g_commandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Grandfathers_Blessing),
            ToInteger(CommandTrait::Living_Plague),
            ToInteger(CommandTrait::Hulking_Physique),
            ToInteger(CommandTrait::Bloated_With_Corruption),
            ToInteger(CommandTrait::Avalanch_Of_Rotten_Flesh),
            ToInteger(CommandTrait::Resilient),
            ToInteger(CommandTrait::Grandfathers_Blessing),
            ToInteger(CommandTrait::Living_Plague),
            ToInteger(CommandTrait::Hulking_Physique),
            ToInteger(CommandTrait::Tainted_Corruptor),
            ToInteger(CommandTrait::Nurgling_Infestation),
            ToInteger(CommandTrait::Pestilent_Breath),
            ToInteger(CommandTrait::Grandfathers_Blessing),
            ToInteger(CommandTrait::Living_Plague),
            ToInteger(CommandTrait::Hulking_Physique),
            ToInteger(CommandTrait::Hideous_Visage),
            ToInteger(CommandTrait::Overpowering_Stench),
            ToInteger(CommandTrait::Virulent_Contagion)
    };

    constexpr std::array<int, 7> g_daemonCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Grandfathers_Blessing),
            ToInteger(CommandTrait::Living_Plague),
            ToInteger(CommandTrait::Hulking_Physique),
            ToInteger(CommandTrait::Tainted_Corruptor),
            ToInteger(CommandTrait::Nurgling_Infestation),
            ToInteger(CommandTrait::Pestilent_Breath)
    };

    constexpr std::array<int, 10> g_mortalRotbringerCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Grandfathers_Blessing),
            ToInteger(CommandTrait::Living_Plague),
            ToInteger(CommandTrait::Hulking_Physique),
            ToInteger(CommandTrait::Bloated_With_Corruption),
            ToInteger(CommandTrait::Avalanch_Of_Rotten_Flesh),
            ToInteger(CommandTrait::Resilient),
            ToInteger(CommandTrait::Hideous_Visage),
            ToInteger(CommandTrait::Overpowering_Stench),
            ToInteger(CommandTrait::Virulent_Contagion)
    };

    constexpr std::array<int, 19> g_artefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::The_Splithorn_Helm),
            ToInteger(Artefact::Muttergrub),
            ToInteger(Artefact::Rustfang),
            ToInteger(Artefact::Flesh_Pealer),
            ToInteger(Artefact::The_Bileheart),
            ToInteger(Artefact::The_Fecund_Flask),
            ToInteger(Artefact::Noxious_Nexus),
            ToInteger(Artefact::Nurgles_Nail),
            ToInteger(Artefact::The_Bountiful_Swarm),
            ToInteger(Artefact::The_Witherstave),
            ToInteger(Artefact::Tome_Of_A_Thousand_Poxes),
            ToInteger(Artefact::The_Endless_Gift),
            ToInteger(Artefact::The_Virulent_Blade),
            ToInteger(Artefact::The_Foetid_Shroud),
            ToInteger(Artefact::Sublucus_Stenchplate),
            ToInteger(Artefact::The_Eye_Of_Nurgle),
            ToInteger(Artefact::The_Carrion_Dirge),
            ToInteger(Artefact::The_Shield_Of_Growths)
    };

    constexpr std::array<int, 7> g_daemonArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Noxious_Nexus),
            ToInteger(Artefact::Nurgles_Nail),
            ToInteger(Artefact::The_Bountiful_Swarm),
            ToInteger(Artefact::The_Witherstave),
            ToInteger(Artefact::Tome_Of_A_Thousand_Poxes),
            ToInteger(Artefact::The_Endless_Gift)
    };

    constexpr std::array<int, 13> g_mortalRotbringerArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::The_Splithorn_Helm),
            ToInteger(Artefact::Muttergrub),
            ToInteger(Artefact::Rustfang),
            ToInteger(Artefact::Flesh_Pealer),
            ToInteger(Artefact::The_Bileheart),
            ToInteger(Artefact::The_Virulent_Blade),
            ToInteger(Artefact::The_Foetid_Shroud),
            ToInteger(Artefact::Sublucus_Stenchplate),
            ToInteger(Artefact::The_Eye_Of_Nurgle),
            ToInteger(Artefact::The_Carrion_Dirge),
            ToInteger(Artefact::The_Shield_Of_Growths)
    };

    constexpr std::array<int, 4> g_daemonLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Favoured_Poxes),
            ToInteger(Lore::Glorious_Afflictions),
            ToInteger(Lore::Sumptuous_Pestilence)
    };

    constexpr std::array<int, 7> g_mortalRotbringerLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Blades_Of_Putrification),
            ToInteger(Lore::Rancid_Visitations),
            ToInteger(Lore::Gift_Of_Contagion),
            ToInteger(Lore::Magnificent_Buboes),
            ToInteger(Lore::Plague_Squall),
            ToInteger(Lore::Cloying_Quagmire)
    };

} // namespace Nurgle
