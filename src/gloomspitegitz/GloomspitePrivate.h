/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef GLOOMSPITEPRIVATE_H
#define GLOOMSPITEPRIVATE_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <gloomspitegitz/GloomspiteSpells.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace GloomspiteGitz {

    const std::array<int, 7> g_blessingsOfTheBadMoon = {
        to_integer(CommandTrait::None),
        to_integer(CommandTrait::Cunning_Plans),
        to_integer(CommandTrait::Fight_Another_Day),
        to_integer(CommandTrait::Sneaky_Stabba),
        to_integer(CommandTrait::Tough_N_Leathery),
        to_integer(CommandTrait::Dead_Shouty),
        to_integer(CommandTrait::The_Clammy_Hand)
    };

    const std::array<int, 7> g_giftsOfTheGloomspite = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Low_Cunning),
            to_integer(CommandTrait::Spiteful_Git),
            to_integer(CommandTrait::Great_Shaman),
            to_integer(CommandTrait::Dodgy_Character),
            to_integer(CommandTrait::Boss_Shaman),
            to_integer(CommandTrait::Loon_Touched)
    };

    const std::array<int, 7> g_marksOfTheSpiderGodsFavour = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Monstrous_Mount),
            to_integer(CommandTrait::Master_Spider_Rider),
            to_integer(CommandTrait::Ulutating_Battle_Cry),
            to_integer(CommandTrait::Tough_N_Leathery),
            to_integer(CommandTrait::Dead_Shouty),
            to_integer(CommandTrait::Creeping_Assault),
    };

    const std::array<int, 7> g_fortuitousTroggbossTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Tough_As_Rocks),
            to_integer(CommandTrait::Alpha_Trogg),
            to_integer(CommandTrait::Loonskin),
            to_integer(CommandTrait::Pulverising_Grip),
            to_integer(CommandTrait::Mighty_Blow),
            to_integer(CommandTrait::Realmstone_Studded_Hide)
    };

    const std::array<int, 7> g_troglodyticTreasures = {
            to_integer(Artefact::None),
            to_integer(Artefact::Spiteful_Prodder),
            to_integer(Artefact::Backstabbers_Blade),
            to_integer(Artefact::Loonstone_Talisman),
            to_integer(Artefact::The_Pipes_of_Doom),
            to_integer(Artefact::The_Clammy_Cowl),
            to_integer(Artefact::Leering_Gitshield)
    };

    const std::array<int, 4> g_foetidFetishes = {
            to_integer(Artefact::None),
            to_integer(Artefact::Spiteshroom_Familiar),
            to_integer(Artefact::Moonface_Mommet),
            to_integer(Artefact::Staff_of_Sneaky_Stealin)
    };

    const std::array<int, 7> g_venomousValuables = {
            to_integer(Artefact::None),
            to_integer(Artefact::Totem_of_the_Spider_God),
            to_integer(Artefact::Headdress_of_Many_Eyes),
            to_integer(Artefact::The_Black_Fang),
            to_integer(Artefact::Nibblas_Itty_Ring),
            to_integer(Artefact::Earskuttla),
            to_integer(Artefact::Web_Strung_Cloak)
    };

    const std::array<int, 4> g_glintyGubbinzThatTroggothsFound = {
            to_integer(Artefact::None),
            to_integer(Artefact::Shiny_Wotnot),
            to_integer(Artefact::Glowy_Howzit),
            to_integer(Artefact::Pet_Gribbly)
    };


    const std::array<int, 7> g_loreOfTheMoonclans = {
            to_integer(Lore::None),
            to_integer(Lore::Vindictive_Glare),
            to_integer(Lore::Itchy_Nuisance),
            to_integer(Lore::The_Great_Green_Spite),
            to_integer(Lore::The_Hand_of_Gork),
            to_integer(Lore::Squig_Lure),
            to_integer(Lore::Call_da_Moon)
    };

    const std::array<int, 7> g_loreOfTheSpiderFangs = {
            to_integer(Lore::None),
            to_integer(Lore::Deadly_Webbing),
            to_integer(Lore::Venomous_Spiderlings),
            to_integer(Lore::Scuttling_Terrors),
            to_integer(Lore::Sneaky_Distraction),
            to_integer(Lore::Curse_of_da_Spider_God),
            to_integer(Lore::Gift_of_da_Spider_God)
    };

} // namespace GloomspiteGitz

#endif //GLOOMSPITEPRIVATE_H