/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NURGLEPRIVATE_H
#define NURGLEPRIVATE_H

#include <nurgle/Nurgle.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Nurgle {

    constexpr std::array<int, 19> g_commandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Grandfathers_Blessing),
            to_integer(CommandTrait::Living_Plague),
            to_integer(CommandTrait::Hulking_Physique),
            to_integer(CommandTrait::Bloated_with_Corruption),
            to_integer(CommandTrait::Avalanch_of_Rotten_Flesh),
            to_integer(CommandTrait::Resilient),
            to_integer(CommandTrait::Grandfathers_Blessing),
            to_integer(CommandTrait::Living_Plague),
            to_integer(CommandTrait::Hulking_Physique),
            to_integer(CommandTrait::Tainted_Corruptor),
            to_integer(CommandTrait::Nurgling_Infestation),
            to_integer(CommandTrait::Pestilent_Breath),
            to_integer(CommandTrait::Grandfathers_Blessing),
            to_integer(CommandTrait::Living_Plague),
            to_integer(CommandTrait::Hulking_Physique),
            to_integer(CommandTrait::Hideous_Visage),
            to_integer(CommandTrait::Overpowering_Stench),
            to_integer(CommandTrait::Virulent_Contagion)
    };

    constexpr std::array<int, 7> g_daemonCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Grandfathers_Blessing),
            to_integer(CommandTrait::Living_Plague),
            to_integer(CommandTrait::Hulking_Physique),
            to_integer(CommandTrait::Tainted_Corruptor),
            to_integer(CommandTrait::Nurgling_Infestation),
            to_integer(CommandTrait::Pestilent_Breath)
    };

    constexpr std::array<int, 10> g_mortalRotbringerCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Grandfathers_Blessing),
            to_integer(CommandTrait::Living_Plague),
            to_integer(CommandTrait::Hulking_Physique),
            to_integer(CommandTrait::Bloated_with_Corruption),
            to_integer(CommandTrait::Avalanch_of_Rotten_Flesh),
            to_integer(CommandTrait::Resilient),
            to_integer(CommandTrait::Hideous_Visage),
            to_integer(CommandTrait::Overpowering_Stench),
            to_integer(CommandTrait::Virulent_Contagion)
    };

    constexpr std::array<int, 19> g_artefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::The_Splithorn_Helm),
            to_integer(Artefact::Muttergrub),
            to_integer(Artefact::Rustfang),
            to_integer(Artefact::Flesh_Pealer),
            to_integer(Artefact::The_Bileheart),
            to_integer(Artefact::The_Fecund_Flask),
            to_integer(Artefact::Noxious_Nexus),
            to_integer(Artefact::Nurgles_Nail),
            to_integer(Artefact::The_Bountiful_Swarm),
            to_integer(Artefact::The_Witherstave),
            to_integer(Artefact::Tome_of_a_Thousand_Poxes),
            to_integer(Artefact::The_Endless_Gift),
            to_integer(Artefact::The_Virulent_Blade),
            to_integer(Artefact::The_Foetid_Shroud),
            to_integer(Artefact::Sublucus_Stenchplate),
            to_integer(Artefact::The_Eye_of_Nurgle),
            to_integer(Artefact::The_Carrion_Dirge),
            to_integer(Artefact::The_Shield_of_Growths)
    };

    constexpr std::array<int, 7> g_daemonArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Noxious_Nexus),
            to_integer(Artefact::Nurgles_Nail),
            to_integer(Artefact::The_Bountiful_Swarm),
            to_integer(Artefact::The_Witherstave),
            to_integer(Artefact::Tome_of_a_Thousand_Poxes),
            to_integer(Artefact::The_Endless_Gift)
    };

    constexpr std::array<int, 13> g_mortalRotbringerArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::The_Splithorn_Helm),
            to_integer(Artefact::Muttergrub),
            to_integer(Artefact::Rustfang),
            to_integer(Artefact::Flesh_Pealer),
            to_integer(Artefact::The_Bileheart),
            to_integer(Artefact::The_Virulent_Blade),
            to_integer(Artefact::The_Foetid_Shroud),
            to_integer(Artefact::Sublucus_Stenchplate),
            to_integer(Artefact::The_Eye_of_Nurgle),
            to_integer(Artefact::The_Carrion_Dirge),
            to_integer(Artefact::The_Shield_of_Growths)
    };

    constexpr std::array<int, 4> g_daemonLore = {
            to_integer(Lore::None),
            to_integer(Lore::Favoured_Poxes),
            to_integer(Lore::Glorious_Afflictions),
            to_integer(Lore::Sumptuous_Pestilence)
    };

    constexpr std::array<int, 7> g_mortalRotbringerLore = {
            to_integer(Lore::None),
            to_integer(Lore::Blades_of_Putrification),
            to_integer(Lore::Rancid_Visitations),
            to_integer(Lore::Gift_of_Contagion),
            to_integer(Lore::Magnificent_Buboes),
            to_integer(Lore::Plague_Squall),
            to_integer(Lore::Cloying_Quagmire)
    };

} // namespace Nurgle

#endif //NURGLEPRIVATE_H