/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SONSOFBEHEHMETPRIVATE_H
#define SONSOFBEHEHMETPRIVATE_H

#include <behemat/SonsOfBehemat.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace SonsOfBehemat {

    constexpr std::array<int, 3> g_tribe = {
            ToInteger(Tribe::Taker),
            ToInteger(Tribe::Stomper),
            ToInteger(Tribe::Breaker)
    };

    constexpr std::array<int, 7> g_takersCommandTrait = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Monstrously_Tough),
            ToInteger(CommandTrait::Old_And_Gnarly),
            ToInteger(CommandTrait::Louder_Than_Words),
            ToInteger(CommandTrait::Strong_Right_Foot),
            ToInteger(CommandTrait::Very_Acquisitive),
            ToInteger(CommandTrait::Extremely_Intimidating)
    };

    constexpr std::array<int, 7> g_stompersCommandTrait = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Monstrously_Tough),
            ToInteger(CommandTrait::Old_And_Gnarly),
            ToInteger(CommandTrait::Louder_Than_Words),
            ToInteger(CommandTrait::Inescapable_Grip),
            ToInteger(CommandTrait::Very_Shouty),
            ToInteger(CommandTrait::Eager_For_The_Fight)
    };

    constexpr std::array<int, 7> g_breakerCommandTrait = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Monstrously_Tough),
            ToInteger(CommandTrait::Old_And_Gnarly),
            ToInteger(CommandTrait::Louder_Than_Words),
            ToInteger(CommandTrait::Extremely_Bitter),
            ToInteger(CommandTrait::Smasher),
            ToInteger(CommandTrait::Sees_Red)
    };

    constexpr std::array<int, 7> g_takerArtefact = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Jaws_Of_The_Mogalodon),
            ToInteger(Artefact::Wallopin_Tentacle),
            ToInteger(Artefact::Jar_Of_Burny_Grog),
            ToInteger(Artefact::Net_Of_The_Beast_Reaver),
            ToInteger(Artefact::Glowy_Lantern),
            ToInteger(Artefact::Krakenskin_Sandals)
    };

    constexpr std::array<int, 4> g_stomperArtefact = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Ironweld_Cestus),
            ToInteger(Artefact::Club_Of_The_First_Oak),
            ToInteger(Artefact::Mantle_Of_The_Destroyer)
    };

    constexpr std::array<int, 4> g_breakerArtefact = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Enchanted_Portcullis),
            ToInteger(Artefact::The_Great_Wrecka),
            ToInteger(Artefact::Kingslaughter_Cowl)
    };

    constexpr std::array<int, 7> g_loathings = {
            ToInteger(FierceLoathing::None),
            ToInteger(FierceLoathing::Bossy_Pants_And_Clever_Clogs),
            ToInteger(FierceLoathing::Idiots_With_Flags),
            ToInteger(FierceLoathing::Shiny_Uns),
            ToInteger(FierceLoathing::Crowds),
            ToInteger(FierceLoathing::Wannabes),
            ToInteger(FierceLoathing::Piggybackers)
    };

} // namespace SonsOfBehemat

#endif //SONSOFBEHEHMETPRIVATE_H