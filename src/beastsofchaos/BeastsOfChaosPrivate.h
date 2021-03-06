/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include "../AoSSimPrivate.h"
#include <array>

namespace BeastsOfChaos {

    constexpr std::array<int, 4> g_greatFray = {
            ToInteger(Greatfray::None),
            ToInteger(Greatfray::Allherd),
            ToInteger(Greatfray::Darkwalkers),
            ToInteger(Greatfray::Gavespawn)
    };

    constexpr std::array<int, 7> g_brayherdCommandTrait = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Beastial_Cunning),
            ToInteger(CommandTrait::Indomitable_Beast),
            ToInteger(CommandTrait::Apex_Predator),
            ToInteger(CommandTrait::Malevolent_Despoiler),
            ToInteger(CommandTrait::Oracle_Of_The_Dark_Tongue),
            ToInteger(CommandTrait::Shadowpelt),
    };

    constexpr std::array<int, 7> g_warherdCommandTrait = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Eater_Of_Heroes),
            ToInteger(CommandTrait::Rampant_Juggernaut),
            ToInteger(CommandTrait::Gorger),
            ToInteger(CommandTrait::Gouge_Tusks),
            ToInteger(CommandTrait::Roaring_Brute),
            ToInteger(CommandTrait::Rugged_Hide),
    };

    constexpr std::array<int, 7> g_thunderscornCommandTrait = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Tempestuous_Tyrant),
            ToInteger(CommandTrait::Magnetic_Monstrosity),
            ToInteger(CommandTrait::Father_Of_The_Storm),
            ToInteger(CommandTrait::Lightning_Fast_Monstrosity),
            ToInteger(CommandTrait::Adamantine_Scales),
            ToInteger(CommandTrait::Ancient_Beyond_Knowledge),
    };

    constexpr std::array<int, 7> g_brayherdArtefact = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Ramhorn_Helm),
            ToInteger(Artefact::Brayblast_Trumpet),
            ToInteger(Artefact::The_Knowing_Eye),
            ToInteger(Artefact::Volcanic_Axe),
            ToInteger(Artefact::Bleating_Gnarlstaff),
            ToInteger(Artefact::Troggoth_Hide_Cloak),
    };

    constexpr std::array<int, 7> g_warherdArtefact = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Cleaver_Of_The_Brass_Bell),
            ToInteger(Artefact::Gilded_Horns),
            ToInteger(Artefact::Glyph_Etched_Talisman),
            ToInteger(Artefact::Blackened_Armour_Of_Chaos),
            ToInteger(Artefact::Champions_Doomcloak),
            ToInteger(Artefact::Herdstone_Shard),
    };

    constexpr std::array<int, 7> g_thunderscornArtefact = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Ancestral_Azyrite_Blade),
            ToInteger(Artefact::Lightning_Chained_Bracers),
            ToInteger(Artefact::Thunderstrike_Lodestone),
            ToInteger(Artefact::Horn_Of_The_Tempest),
            ToInteger(Artefact::Tanglehorn_Familiars),
            ToInteger(Artefact::Ruinous_Icon),
    };

    constexpr std::array<int, 7> g_loreOfTheTwistedWilds{
            ToInteger(Lore::None),
            ToInteger(Lore::Viletide),
            ToInteger(Lore::Vicious_Stanglethorns),
            ToInteger(Lore::Savage_Dominion),
            ToInteger(Lore::Tendrils_Of_Atrophy),
            ToInteger(Lore::Wild_Rampage),
            ToInteger(Lore::Titanic_Fury)
    };

    constexpr std::array<int, 4> g_loreOfTheDarkStorms {
            ToInteger(Lore::None),
            ToInteger(Lore::Thunderwave),
            ToInteger(Lore::Hailstorm),
            ToInteger(Lore::Sundering_Blades)
    };

} // namespace BeastsOfChaos
