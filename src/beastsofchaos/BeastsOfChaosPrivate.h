/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BEASTSOFCHAOSPRIVATE_H
#define BEASTSOFCHAOSPRIVATE_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace BeastsOfChaos {

    constexpr std::array<int, 4> g_greatFray = {
        to_integer(Greatfray::None),
        to_integer(Greatfray::Allherd),
        to_integer(Greatfray::Darkwalkers),
        to_integer(Greatfray::Gavespawn)
    };

    constexpr std::array<int, 7> g_brayherdCommandTrait = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Beastial_Cunning),
            to_integer(CommandTrait::Indomitable_Beast),
            to_integer(CommandTrait::Apex_Predator),
            to_integer(CommandTrait::Malevolent_Despoiler),
            to_integer(CommandTrait::Oracle_of_the_Dark_Tongue),
            to_integer(CommandTrait::Shadowpelt),
    };

    constexpr std::array<int, 7> g_warherdCommandTrait = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Eater_of_Heroes),
            to_integer(CommandTrait::Rampant_Juggernaut),
            to_integer(CommandTrait::Gorger),
            to_integer(CommandTrait::Gouge_Tusks),
            to_integer(CommandTrait::Roaring_Brute),
            to_integer(CommandTrait::Rugged_Hide),
    };

    constexpr std::array<int, 7> g_thunderscornCommandTrait = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Tempestuous_Tyrant),
            to_integer(CommandTrait::Magnetic_Monstrosity),
            to_integer(CommandTrait::Father_of_the_Storm),
            to_integer(CommandTrait::Lightning_Fast_Monstrosity),
            to_integer(CommandTrait::Adamantine_Scales),
            to_integer(CommandTrait::Ancient_Beyond_Knowledge),
    };

    constexpr std::array<int, 7> g_brayherdArtefact = {
            to_integer(Artefact::None),
            to_integer(Artefact::Ramhorn_Helm),
            to_integer(Artefact::Brayblast_Trumpet),
            to_integer(Artefact::The_Knowing_Eye),
            to_integer(Artefact::Volcanic_Axe),
            to_integer(Artefact::Bleating_Gnarlstaff),
            to_integer(Artefact::Troggoth_Hide_Cloak),
    };

    constexpr std::array<int, 7> g_warherdArtefact = {
            to_integer(Artefact::None),
            to_integer(Artefact::Cleaver_of_the_Brass_Bell),
            to_integer(Artefact::Gilded_Horns),
            to_integer(Artefact::Glyph_Etched_Talisman),
            to_integer(Artefact::Blackened_Armour_of_Chaos),
            to_integer(Artefact::Champions_Doomcloak),
            to_integer(Artefact::Herdstone_Shard),
    };

    constexpr std::array<int, 7> g_thunderscornArtefact = {
            to_integer(Artefact::None),
            to_integer(Artefact::Ancestral_Azyrite_Blade),
            to_integer(Artefact::Lightning_Chained_Bracers),
            to_integer(Artefact::Thunderstrike_Lodestone),
            to_integer(Artefact::Horn_of_the_Tempest),
            to_integer(Artefact::Tanglehorn_Familiars),
            to_integer(Artefact::Ruinous_Icon),
    };

    constexpr std::array<int, 7> g_loreOfTheTwistedWilds{
            to_integer(Lore::None),
            to_integer(Lore::Viletide),
            to_integer(Lore::Vicious_Stanglethorns),
            to_integer(Lore::Savage_Dominion),
            to_integer(Lore::Tendrils_of_Atrophy),
            to_integer(Lore::Wild_Rampage),
            to_integer(Lore::Titanic_Fury)
    };

    constexpr std::array<int, 4> g_loreOfTheDarkStorms {
            to_integer(Lore::None),
            to_integer(Lore::Thunderwave),
            to_integer(Lore::Hailstorm),
            to_integer(Lore::Sundering_Blades)
    };

} // namespace BeastsOfChaos

#endif //BEASTSOFCHAOSPRIVATE_H