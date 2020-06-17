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

    constexpr std::array<int, 3> g_host = {
            to_integer(Host::Invaders),
            to_integer(Host::Pretenders),
            to_integer(Host::Godseekers)
    };

    constexpr std::array<int, 19> g_commandTraits = {
            to_integer(CommandTrait::None),

            // Invaders
            to_integer(CommandTrait::Best_of_the_Best),
            to_integer(CommandTrait::Glory_Hog),
            to_integer(CommandTrait::Hurler_of_Obscenities),
            to_integer(CommandTrait::Territorial),
            to_integer(CommandTrait::Skin_Taker),
            to_integer(CommandTrait::Extra_Wound),

            // Pretenders
            to_integer(CommandTrait::Strength_of_Godhood),
            to_integer(CommandTrait::Monarch_of_Lies),
            to_integer(CommandTrait::True_Child_of_Slaanesh),
            to_integer(CommandTrait::Strongest_Alone),
            to_integer(CommandTrait::Hunter_of_Godbeasts),
            to_integer(CommandTrait::Inspirer),

            // Godseekers
            to_integer(CommandTrait::Hunter_Supreme),
            to_integer(CommandTrait::Thrill_Seeker),
            to_integer(CommandTrait::Into_the_Fray),
            to_integer(CommandTrait::Trail_Sniffer),
            to_integer(CommandTrait::Symphoniac),
            to_integer(CommandTrait::Speed_Chaser)
    };

    constexpr std::array<int, 19> g_artefacts{
            to_integer(Artefact::None),

            // Invaders
            to_integer(Artefact::The_Rod_of_Misrule),
            to_integer(Artefact::Rapier_of_Ecstatic_Conquest),
            to_integer(Artefact::Whip_of_Subversion),
            to_integer(Artefact::Icon_of_Infinite_Excess),
            to_integer(Artefact::Fallacious_Gift),
            to_integer(Artefact::The_Beguiling_Gem),

            // Pretenders
            to_integer(Artefact::The_Crown_of_Dark_Secrets),
            to_integer(Artefact::Pendant_of_Slaanesh),
            to_integer(Artefact::Sliverslash),
            to_integer(Artefact::Sceptre_of_Domination),
            to_integer(Artefact::Breathtaker),
            to_integer(Artefact::Mask_of_Spiteful_Beauty),

            // Godseekers
            to_integer(Artefact::Cameo_of_the_Dark_Prince),
            to_integer(Artefact::Girdle_of_the_Realm_Racer),
            to_integer(Artefact::Threnody_Voicebox),
            to_integer(Artefact::Lash_of_Despair),
            to_integer(Artefact::Enrapturing_Circlet),
            to_integer(Artefact::Binding_of_Slaanesh)
    };

    constexpr std::array<int, 7> g_daemonLore = {
            to_integer(Lore::None),
            to_integer(Lore::Lash_of_Slaanesh),
            to_integer(Lore::Pavane_of_Slaanesh),
            to_integer(Lore::Hysterical_Frenzy),
            to_integer(Lore::Soulslice_Shards),
            to_integer(Lore::Phantasmagoria),
            to_integer(Lore::Bord_of_Damnation),
    };

    constexpr std::array<int, 4> g_greaterDaemonLore = {
            to_integer(Lore::None),
            to_integer(Lore::Song_of_Secrets),
            to_integer(Lore::Progeny_of_Damnation),
            to_integer(Lore::Slothful_Stupor)
    };

    constexpr std::array<int, 4> g_mortalLore = {
            to_integer(Lore::None),
            to_integer(Lore::Battle_Rapture),
            to_integer(Lore::Dark_Delusions),
            to_integer(Lore::Hellshriek)
    };

} // namespace Slaanesh

#endif //SLAANESHPRIVATE_H