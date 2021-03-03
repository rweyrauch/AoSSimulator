/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAANESHPRIVATE_H
#define SLAANESHPRIVATE_H

#include <slaanesh/SlaaneshBase.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace Slaanesh {

    constexpr std::array<int, 6> g_host = {
            ToInteger(Host::Invaders),
            ToInteger(Host::Pretenders),
            ToInteger(Host::Godseekers),
            ToInteger(Host::Lurid_Haze_Invaders),
            ToInteger(Host::Faultless_Blades_Pretenders),
            ToInteger(Host::Scarlet_Cavalcade_Godseekers)
    };

    constexpr std::array<int, 19> g_commandTraits = {
            ToInteger(CommandTrait::None),

            // Invaders
            ToInteger(CommandTrait::Best_Of_The_Best),
            ToInteger(CommandTrait::Glory_Hog),
            ToInteger(CommandTrait::Hurler_Of_Obscenities),
            ToInteger(CommandTrait::Territorial),
            ToInteger(CommandTrait::Skin_Taker),
            ToInteger(CommandTrait::Delusions_Of_Infallibility),

            // Pretenders
            ToInteger(CommandTrait::Strength_Of_Godhood),
            ToInteger(CommandTrait::Monarch_Of_Lies),
            ToInteger(CommandTrait::Craving_Stare),
            ToInteger(CommandTrait::Strongest_Alone),
            ToInteger(CommandTrait::Hunter_Of_Godbeasts),
            ToInteger(CommandTrait::Inspirer),

            // Godseekers
            ToInteger(CommandTrait::Hunter_Supreme),
            ToInteger(CommandTrait::Sweeping_Slash),
            ToInteger(CommandTrait::Into_The_Fray),
            ToInteger(CommandTrait::Trail_Sniffer),
            ToInteger(CommandTrait::Symphoniac),
            ToInteger(CommandTrait::Speed_Chaser)
    };

    constexpr std::array<int, 19> g_artefacts{
            ToInteger(Artefact::None),

            // Invaders
            ToInteger(Artefact::The_Rod_Of_Misrule),
            ToInteger(Artefact::Rapier_Of_Ecstatic_Conquest),
            ToInteger(Artefact::Whip_Of_Subversion),
            ToInteger(Artefact::Icon_Of_Infinite_Excess),
            ToInteger(Artefact::Fallacious_Gift),
            ToInteger(Artefact::The_Beguiling_Gem),

            // Pretenders
            ToInteger(Artefact::The_Crown_Of_Dark_Secrets),
            ToInteger(Artefact::Pendant_Of_Slaanesh),
            ToInteger(Artefact::Sliverslash),
            ToInteger(Artefact::Sceptre_Of_Domination),
            ToInteger(Artefact::Breathtaker),
            ToInteger(Artefact::Mask_Of_Spiteful_Beauty),

            // Godseekers
            ToInteger(Artefact::Cameo_Of_The_Dark_Prince),
            ToInteger(Artefact::Girdle_Of_The_Realm_Racer),
            ToInteger(Artefact::Threnody_Voicebox),
            ToInteger(Artefact::Lash_Of_Despair),
            ToInteger(Artefact::Enrapturing_Circlet),
            ToInteger(Artefact::Binding_Of_Slaanesh)
    };

    constexpr std::array<int, 7> g_daemonLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Lash_Of_Slaanesh),
            ToInteger(Lore::Pavane_Of_Slaanesh),
            ToInteger(Lore::Hysterical_Frenzy),
            ToInteger(Lore::Soulslice_Shards),
            ToInteger(Lore::Phantasmagoria),
            ToInteger(Lore::Born_Of_Damnation),
    };

    constexpr std::array<int, 4> g_greaterDaemonLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Paths_Of_The_Dark_Prince),
            ToInteger(Lore::Progeny_Of_Damnation),
            ToInteger(Lore::Slothful_Stupor)
    };

    constexpr std::array<int, 4> g_mortalLore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Battle_Rapture),
            ToInteger(Lore::Judgement_Of_Excess),
            ToInteger(Lore::Dark_Delusions),
    };

} // namespace Slaanesh

#endif //SLAANESHPRIVATE_H