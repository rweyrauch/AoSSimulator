/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHARADRONPRIVATE_H
#define KHARADRONPRIVATE_H

#include <kharadron/KharadronBase.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace KharadronOverlords {

    constexpr std::array<int, 7> g_skyport = {
            to_integer(Skyport::None),
            to_integer(Skyport::Barak_Nar),
            to_integer(Skyport::Barak_Zilfin),
            to_integer(Skyport::Barak_Zon),
            to_integer(Skyport::Barak_Urbaz),
            to_integer(Skyport::Barak_Mhornar),
            to_integer(Skyport::Barak_Thryng)
    };

    constexpr std::array<int, 3> g_artycles = {
            to_integer(Artycle::Honour_Is_Everything),
            to_integer(Artycle::Master_the_Skies),
            to_integer(Artycle::Settle_the_Grudges)
    };

    constexpr std::array<int, 3> g_amendments = {
            to_integer(Amendment::Always_Take_What_You_Are_Owed),
            to_integer(Amendment::Prosecute_Wars_With_All_Haste),
            to_integer(Amendment::Trust_To_Your_Guns)
    };

    constexpr std::array<int, 3> g_footnotes = {
            to_integer(Footnote::Theres_No_Reward_Without_Risk),
            to_integer(Footnote::Theres_No_Trading_With_Some_People),
            to_integer(Footnote::Without_Our_Ships_We_Are_Naught)
    };

    constexpr std::array<int, 7> g_admiralCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Wealthy),
            to_integer(CommandTrait::Tough_as_Old_Boots),
            to_integer(CommandTrait::Grudgebearer),
            to_integer(CommandTrait::Cunning_Fleetmaster),
            to_integer(CommandTrait::War_Wounds),
            to_integer(CommandTrait::A_Scholar_and_an_Arkanaut)
    };

    constexpr std::array<int, 7> g_endrinmasterCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Wealthy),
            to_integer(CommandTrait::Tough_as_Old_Boots),
            to_integer(CommandTrait::Grudgebearer),
            to_integer(CommandTrait::Grandmaster),
            to_integer(CommandTrait::Great_Tinkerer),
            to_integer(CommandTrait::Endrinprofessor)
    };

    constexpr std::array<int, 7> g_navigatorCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Wealthy),
            to_integer(CommandTrait::Tough_as_Old_Boots),
            to_integer(CommandTrait::Stormcaller),
            to_integer(CommandTrait::Ride_the_Winds),
            to_integer(CommandTrait::Sceptic),
            to_integer(CommandTrait::Diviner)
    };

    constexpr std::array<int, 7> g_khemistCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Wealthy),
            to_integer(CommandTrait::Tough_as_Old_Boots),
            to_integer(CommandTrait::Grudgebearer),
            to_integer(CommandTrait::A_Nose_for_Gold),
            to_integer(CommandTrait::Genius_in_the_Making),
            to_integer(CommandTrait::Collector)
    };

    constexpr std::array<int, 7> g_admiralArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Masterwrought_Armour),
            to_integer(Artefact::Hammer_of_Aetheric_Might),
            to_integer(Artefact::Gattlesons_Endless_Repeater),
            to_integer(Artefact::Rune_of_Mark),
            to_integer(Artefact::Flask_of_Vintage_Amberwhisky),
            to_integer(Artefact::Proclamtor_Mask_Hailer)
    };

    constexpr std::array<int, 4> g_navigatorArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Cyclonic_Aethometer),
            to_integer(Artefact::Svaregg_Stein_Illuminator_Flarepistol),
            to_integer(Artefact::Voidstone_Orb)
    };

    constexpr std::array<int, 4> g_endrinmasterArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Cogmonculus),
            to_integer(Artefact::Aetherquartz_Monolens),
            to_integer(Artefact::Seismic_Shock_Gauntlets)
    };

    constexpr std::array<int, 4> g_dirigibleArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Aether_Injection_Boosters),
            to_integer(Artefact::Phosphorite_Bomblets),
            to_integer(Artefact::Miniaturized_Aethermatic_Repulsion_Field)
    };

    constexpr std::array<int, 4> g_khemistArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Emergency_Ventplates),
            to_integer(Artefact::Caustic_Anatomiser),
            to_integer(Artefact::Spell_in_a_Bottle)
    };

    constexpr std::array<int, 7> g_ironcladEndrinworks = {
            to_integer(Endrinwork::None),
            to_integer(Endrinwork::The_Last_Word),
            to_integer(Endrinwork::Hegsson_Solutions_Old_Reliable_Hullplates),
            to_integer(Endrinwork::Ebullient_Buoyancy_Aid),
            to_integer(Endrinwork::Prudency_Chutes),
            to_integer(Endrinwork::Magnificent_Omniscope),
            to_integer(Endrinwork::Zonbarcorp_Dealbreaker_Battle_Ram)
    };

    constexpr std::array<int, 4> g_frigateEndrinworks = {
            to_integer(Endrinwork::None),
            to_integer(Endrinwork::Prudency_Chutes),
            to_integer(Endrinwork::Magnificent_Omniscope),
            to_integer(Endrinwork::Malefic_Skymines)
    };

    constexpr std::array<int, 4> g_gunhaulerEndrinworks = {
            to_integer(Endrinwork::None),
            to_integer(Endrinwork::Iggrind_Kaz_Surge_Injection_Endrin_Mk_IV),
            to_integer(Endrinwork::Zonbarcorp_Debtsettler_Spar_Torpedo),
            to_integer(Endrinwork::Coalbeards_Collapsible_Compartments)
    };

} // namespace KharadronOverlords

#endif // KHARADRONPRIVATE_H