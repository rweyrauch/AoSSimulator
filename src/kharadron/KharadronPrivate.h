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
            ToInteger(Skyport::None),
            ToInteger(Skyport::Barak_Nar),
            ToInteger(Skyport::Barak_Zilfin),
            ToInteger(Skyport::Barak_Zon),
            ToInteger(Skyport::Barak_Urbaz),
            ToInteger(Skyport::Barak_Mhornar),
            ToInteger(Skyport::Barak_Thryng)
    };

    constexpr std::array<int, 3> g_artycles = {
            ToInteger(Artycle::Honour_Is_Everything),
            ToInteger(Artycle::Master_The_Skies),
            ToInteger(Artycle::Settle_The_Grudges)
    };

    constexpr std::array<int, 3> g_amendments = {
            ToInteger(Amendment::Always_Take_What_You_Are_Owed),
            ToInteger(Amendment::Prosecute_Wars_With_All_Haste),
            ToInteger(Amendment::Trust_To_Your_Guns)
    };

    constexpr std::array<int, 3> g_footnotes = {
            ToInteger(Footnote::Theres_No_Reward_Without_Risk),
            ToInteger(Footnote::Theres_No_Trading_With_Some_People),
            ToInteger(Footnote::Without_Our_Ships_We_Are_Naught)
    };

    constexpr std::array<int, 7> g_admiralCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Wealthy),
            ToInteger(CommandTrait::Tough_As_Old_Boots),
            ToInteger(CommandTrait::Grudgebearer),
            ToInteger(CommandTrait::Cunning_Fleetmaster),
            ToInteger(CommandTrait::War_Wounds),
            ToInteger(CommandTrait::A_Scholar_And_An_Arkanaut)
    };

    constexpr std::array<int, 7> g_endrinmasterCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Wealthy),
            ToInteger(CommandTrait::Tough_As_Old_Boots),
            ToInteger(CommandTrait::Grudgebearer),
            ToInteger(CommandTrait::Grandmaster),
            ToInteger(CommandTrait::Great_Tinkerer),
            ToInteger(CommandTrait::Endrinprofessor)
    };

    constexpr std::array<int, 7> g_navigatorCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Wealthy),
            ToInteger(CommandTrait::Tough_As_Old_Boots),
            ToInteger(CommandTrait::Stormcaller),
            ToInteger(CommandTrait::Ride_The_Winds),
            ToInteger(CommandTrait::Sceptic),
            ToInteger(CommandTrait::Diviner)
    };

    constexpr std::array<int, 7> g_khemistCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Wealthy),
            ToInteger(CommandTrait::Tough_As_Old_Boots),
            ToInteger(CommandTrait::Grudgebearer),
            ToInteger(CommandTrait::A_Nose_For_Gold),
            ToInteger(CommandTrait::Genius_In_The_Making),
            ToInteger(CommandTrait::Collector)
    };

    constexpr std::array<int, 7> g_admiralArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Masterwrought_Armour),
            ToInteger(Artefact::Hammer_Of_Aetheric_Might),
            ToInteger(Artefact::Gattlesons_Endless_Repeater),
            ToInteger(Artefact::Rune_Of_Mark),
            ToInteger(Artefact::Flask_Of_Vintage_Amberwhisky),
            ToInteger(Artefact::Proclamtor_Mask_Hailer)
    };

    constexpr std::array<int, 4> g_navigatorArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Cyclonic_Aethometer),
            ToInteger(Artefact::Svaregg_Stein_Illuminator_Flarepistol),
            ToInteger(Artefact::Voidstone_Orb)
    };

    constexpr std::array<int, 4> g_endrinmasterArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Cogmonculus),
            ToInteger(Artefact::Aetherquartz_Monolens),
            ToInteger(Artefact::Seismic_Shock_Gauntlets)
    };

    constexpr std::array<int, 4> g_dirigibleArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Aether_Injection_Boosters),
            ToInteger(Artefact::Phosphorite_Bomblets),
            ToInteger(Artefact::Miniaturized_Aethermatic_Repulsion_Field)
    };

    constexpr std::array<int, 4> g_khemistArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Emergency_Ventplates),
            ToInteger(Artefact::Caustic_Anatomiser),
            ToInteger(Artefact::Spell_In_A_Bottle)
    };

    constexpr std::array<int, 7> g_ironcladEndrinworks = {
            ToInteger(Endrinwork::None),
            ToInteger(Endrinwork::The_Last_Word),
            ToInteger(Endrinwork::Hegsson_Solutions_Old_Reliable_Hullplates),
            ToInteger(Endrinwork::Ebullient_Buoyancy_Aid),
            ToInteger(Endrinwork::Prudency_Chutes),
            ToInteger(Endrinwork::Magnificent_Omniscope),
            ToInteger(Endrinwork::Zonbarcorp_Dealbreaker_Battle_Ram)
    };

    constexpr std::array<int, 4> g_frigateEndrinworks = {
            ToInteger(Endrinwork::None),
            ToInteger(Endrinwork::Prudency_Chutes),
            ToInteger(Endrinwork::Magnificent_Omniscope),
            ToInteger(Endrinwork::Malefic_Skymines)
    };

    constexpr std::array<int, 4> g_gunhaulerEndrinworks = {
            ToInteger(Endrinwork::None),
            ToInteger(Endrinwork::Iggrind_Kaz_Surge_Injection_Endrin_Mk_Iv),
            ToInteger(Endrinwork::Zonbarcorp_Debtsettler_Spar_Torpedo),
            ToInteger(Endrinwork::Coalbeards_Collapsible_Compartments)
    };

} // namespace KharadronOverlords

#endif // KHARADRONPRIVATE_H