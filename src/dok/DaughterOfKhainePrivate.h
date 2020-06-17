/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DAUGHTEROFKHAINEPRIVATE_H
#define DAUGHTEROFKHAINEPRIVATE_H

#include <dok/DaughterOfKhaine.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace DaughtersOfKhaine {

    constexpr std::array<int, 5> g_temple = {
        to_integer(Temple::None),
        to_integer(Temple::HaggNar),
        to_integer(Temple::DraichiGaneth),
        to_integer(Temple::TheKraith),
        to_integer(Temple::Khailebron)
    };

    constexpr std::array<int, 7> g_commandTraits = {
        to_integer(CommandTrait::None),
        to_integer(CommandTrait::Bathed_in_Blood),
        to_integer(CommandTrait::Zealous_Orator),
        to_integer(CommandTrait::Bloody_Sacrificer),
        to_integer(CommandTrait::Terrifying_Beauty),
        to_integer(CommandTrait::Mistress_of_Poisons),
        to_integer(CommandTrait::True_Believer)
    };

    constexpr std::array<int, 7> g_heroArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Crown_of_Woe),
            to_integer(Artefact::Cursed_Blade),
            to_integer(Artefact::Amulet_of_Dark_Fire),
            to_integer(Artefact::Crone_Blade),
            to_integer(Artefact::Thousand_and_One_Dark_Blessings),
            to_integer(Artefact::Bloodbane_Venom)
    };

    constexpr std::array<int, 7> g_wizardArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Shadow_Stone),
            to_integer(Artefact::Rune_of_Ulgu),
            to_integer(Artefact::The_Mirror_Glaive),
            to_integer(Artefact::Seven_Fold_Shadow),
            to_integer(Artefact::Crystal_Heart),
            to_integer(Artefact::Shade_Claw)
    };

    constexpr std::array<int, 7> g_priestArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Blood_Sigil),
            to_integer(Artefact::Iron_Circlet),
            to_integer(Artefact::Rune_of_Khaine),
            to_integer(Artefact::Crimson_Shard),
            to_integer(Artefact::Khainite_Pendant),
            to_integer(Artefact::Hagbrew)
    };

    constexpr std::array<int, 7> g_lore = {
        to_integer(Lore::None),
        to_integer(Lore::Steed_of_Shadows),
        to_integer(Lore::Pit_of_Shades),
        to_integer(Lore::Mirror_Dance),
        to_integer(Lore::The_Withering),
        to_integer(Lore::Mindrazor),
        to_integer(Lore::Shroud_of_Despair)
    };

    constexpr std::array<int, 7> g_prayers = {
        to_integer(Prayer::None),
        to_integer(Prayer::Catechism_of_Murder),
        to_integer(Prayer::Blessing_of_Khaine),
        to_integer(Prayer::Martyrs_Sacrifice),
        to_integer(Prayer::Crimson_Rejuvination),
        to_integer(Prayer::Covenant_of_the_Iron_Heart),
        to_integer(Prayer::Sacrament_of_Blood)
    };

} // namespace DaughtersOfKhaine

#endif //DAUGHTEROFKHAINEPRIVATE_H