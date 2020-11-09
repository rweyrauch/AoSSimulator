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
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Cunning_Plans),
            ToInteger(CommandTrait::Fight_Another_Day),
            ToInteger(CommandTrait::Sneaky_Stabba),
            ToInteger(CommandTrait::Tough_N_Leathery),
            ToInteger(CommandTrait::Dead_Shouty),
            ToInteger(CommandTrait::The_Clammy_Hand)
    };

    const std::array<int, 7> g_giftsOfTheGloomspite = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Low_Cunning),
            ToInteger(CommandTrait::Spiteful_Git),
            ToInteger(CommandTrait::Great_Shaman),
            ToInteger(CommandTrait::Dodgy_Character),
            ToInteger(CommandTrait::Boss_Shaman),
            ToInteger(CommandTrait::Loon_Touched)
    };

    const std::array<int, 7> g_marksOfTheSpiderGodsFavour = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Monstrous_Mount),
            ToInteger(CommandTrait::Master_Spider_Rider),
            ToInteger(CommandTrait::Ulutating_Battle_Cry),
            ToInteger(CommandTrait::Tough_N_Leathery),
            ToInteger(CommandTrait::Dead_Shouty),
            ToInteger(CommandTrait::Creeping_Assault),
    };

    const std::array<int, 7> g_fortuitousTroggbossTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Tough_As_Rocks),
            ToInteger(CommandTrait::Alpha_Trogg),
            ToInteger(CommandTrait::Loonskin),
            ToInteger(CommandTrait::Pulverising_Grip),
            ToInteger(CommandTrait::Mighty_Blow),
            ToInteger(CommandTrait::Realmstone_Studded_Hide)
    };

    const std::array<int, 7> g_troglodyticTreasures = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Spiteful_Prodder),
            ToInteger(Artefact::Backstabbers_Blade),
            ToInteger(Artefact::Loonstone_Talisman),
            ToInteger(Artefact::The_Pipes_Of_Doom),
            ToInteger(Artefact::The_Clammy_Cowl),
            ToInteger(Artefact::Leering_Gitshield)
    };

    const std::array<int, 4> g_foetidFetishes = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Spiteshroom_Familiar),
            ToInteger(Artefact::Moonface_Mommet),
            ToInteger(Artefact::Staff_Of_Sneaky_Stealin)
    };

    const std::array<int, 7> g_venomousValuables = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Totem_Of_The_Spider_God),
            ToInteger(Artefact::Headdress_Of_Many_Eyes),
            ToInteger(Artefact::The_Black_Fang),
            ToInteger(Artefact::Nibblas_Itty_Ring),
            ToInteger(Artefact::Earskuttla),
            ToInteger(Artefact::Web_Strung_Cloak)
    };

    const std::array<int, 4> g_glintyGubbinzThatTroggothsFound = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Shiny_Wotnot),
            ToInteger(Artefact::Glowy_Howzit),
            ToInteger(Artefact::Pet_Gribbly)
    };


    const std::array<int, 7> g_loreOfTheMoonclans = {
            ToInteger(Lore::None),
            ToInteger(Lore::Vindictive_Glare),
            ToInteger(Lore::Itchy_Nuisance),
            ToInteger(Lore::The_Great_Green_Spite),
            ToInteger(Lore::The_Hand_Of_Gork),
            ToInteger(Lore::Squig_Lure),
            ToInteger(Lore::Call_Da_Moon)
    };

    const std::array<int, 7> g_loreOfTheSpiderFangs = {
            ToInteger(Lore::None),
            ToInteger(Lore::Deadly_Webbing),
            ToInteger(Lore::Venomous_Spiderlings),
            ToInteger(Lore::Scuttling_Terrors),
            ToInteger(Lore::Sneaky_Distraction),
            ToInteger(Lore::Curse_Of_Da_Spider_God),
            ToInteger(Lore::Gift_Of_Da_Spider_God)
    };

} // namespace GloomspiteGitz

#endif //GLOOMSPITEPRIVATE_H