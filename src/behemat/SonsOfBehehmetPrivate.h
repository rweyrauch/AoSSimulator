/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SONSOFBEHEMATPRIVATE_H
#define SONSOFBEHEMATPRIVATE_H

#include <behemat/SonsOfBehemat.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace SonsOfBehemat {

    constexpr std::array<int, 3> g_tribe = {
            to_integer(Tribe::Taker),
            to_integer(Tribe::Stomper),
            to_integer(Tribe::Breaker)
    };

    constexpr std::array<int, 7> g_takersCommandTrait = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Monstrously_Tough),
            to_integer(CommandTrait::Old_and_Gnarly),
            to_integer(CommandTrait::Louder_than_Words),
            to_integer(CommandTrait::Strong_Right_Foot),
            to_integer(CommandTrait::Very_Acquisitive),
            to_integer(CommandTrait::Extremely_Intimidating)
    };

    constexpr std::array<int, 7> g_stompersCommandTrait = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Monstrously_Tough),
            to_integer(CommandTrait::Old_and_Gnarly),
            to_integer(CommandTrait::Louder_than_Words),
            to_integer(CommandTrait::Inescapable_Grip),
            to_integer(CommandTrait::Very_Shouty),
            to_integer(CommandTrait::Eager_for_the_Fight)
    };

    constexpr std::array<int, 7> g_breakerCommandTrait = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Monstrously_Tough),
            to_integer(CommandTrait::Old_and_Gnarly),
            to_integer(CommandTrait::Louder_than_Words),
            to_integer(CommandTrait::Extremely_Bitter),
            to_integer(CommandTrait::Smasher),
            to_integer(CommandTrait::Sees_Red)
    };

    constexpr std::array<int, 7> g_takerArtefact = {
            to_integer(Artefact::None),
            to_integer(Artefact::Jaws_of_the_Mogalodon),
            to_integer(Artefact::Wallopin_Tentacle),
            to_integer(Artefact::Jar_of_Burny_Grog),
            to_integer(Artefact::Net_of_the_Beast_Reaver),
            to_integer(Artefact::Glowy_Lantern),
            to_integer(Artefact::Krakenskin_Sandals)
    };

    constexpr std::array<int, 4> g_stomperArtefact = {
            to_integer(Artefact::None),
            to_integer(Artefact::Ironweld_Cestus),
            to_integer(Artefact::Club_of_the_First_Oak),
            to_integer(Artefact::Mantle_of_the_Destroyer)
    };

    constexpr std::array<int, 4> g_breakerArtefact = {
            to_integer(Artefact::None),
            to_integer(Artefact::Enchanted_Portcullis),
            to_integer(Artefact::The_Great_Wrecka),
            to_integer(Artefact::Kingslaughter_Cowl)
    };

    constexpr std::array<int, 7> g_loathings = {
            to_integer(FierceLoathing::None),
            to_integer(FierceLoathing::Bossy_Pants_and_Clever_Clogs),
            to_integer(FierceLoathing::Idiots_with_Flags),
            to_integer(FierceLoathing::Shiny_Uns),
            to_integer(FierceLoathing::Crowds),
            to_integer(FierceLoathing::Wannabes),
            to_integer(FierceLoathing::Piggybackers)
    };

} // namespace SonsOfBehemat

#endif //SONSOFBEHEMATPRIVATE_H