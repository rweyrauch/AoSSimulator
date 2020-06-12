/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAVESTODARKNESSPRIVATE_H
#define SLAVESTODARKNESSPRIVATE_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace SlavesToDarkness {

    constexpr std::array<int, 4> g_damnedLegion = {
            to_integer(DamnedLegion::Ravagers),
            to_integer(DamnedLegion::Cabalists),
            to_integer(DamnedLegion::Despoilers),
            to_integer(DamnedLegion::Host_of_the_Everchosen)
    };

    constexpr std::array<int, 5> g_markOfChaos = {
            to_integer(MarkOfChaos::Undivided),
            to_integer(MarkOfChaos::Nurgle),
            to_integer(MarkOfChaos::Khorne),
            to_integer(MarkOfChaos::Slaanesh),
            to_integer(MarkOfChaos::Tzeentch)
    };

    constexpr std::array<int, 7> g_lore = {
            to_integer(Lore::None),
            to_integer(Lore::Binding_Damnation),
            to_integer(Lore::Spite_Tongue_Curse),
            to_integer(Lore::Whispers_of_Chaos),
            to_integer(Lore::Mask_of_Darkness),
            to_integer(Lore::Call_to_Glory),
            to_integer(Lore::Ruinous_Vigour)
    };

    constexpr std::array<int, 7> g_ravagersCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Bolstered_by_Hate),
            to_integer(CommandTrait::Unquestioned_Resolve),
            to_integer(CommandTrait::Favoured_of_the_Pantheon),
            to_integer(CommandTrait::Eternal_Vendetta),
            to_integer(CommandTrait::Flames_of_Spite),
            to_integer(CommandTrait::Master_of_Deception)
    };

    constexpr std::array<int, 7> g_cabalistCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Bolstered_by_Hate),
            to_integer(CommandTrait::Lord_of_Terror),
            to_integer(CommandTrait::Favoured_of_the_Pantheon),
            to_integer(CommandTrait::Mighty_Ritualist),
            to_integer(CommandTrait::Blasphemous_Influence),
            to_integer(CommandTrait::All_for_One)
    };

    constexpr std::array<int, 7> g_despoilerCommandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Bolstered_by_Hate),
            to_integer(CommandTrait::Lord_of_Terror),
            to_integer(CommandTrait::Lightning_Reflexes),
            to_integer(CommandTrait::Radiance_of_Dark_Glory),
            to_integer(CommandTrait::Distorting_Miasma),
            to_integer(CommandTrait::Paragon_of_Ruin)
    };

    constexpr std::array<int, 7> g_ravagerArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Hellfire_Sword),
            to_integer(Artefact::Blasphemous_Cuirass),
            to_integer(Artefact::Helm_of_the_Oppressor),
            to_integer(Artefact::Cloak_of_the_Relentless_Conqueror),
            to_integer(Artefact::Mark_of_the_High_Favoured),
            to_integer(Artefact::Desecrator_Gauntlets)
    };

    constexpr std::array<int, 7> g_cabalistArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Soul_Feeder),
            to_integer(Artefact::Black_Athame),
            to_integer(Artefact::Infernal_Puppet),
            to_integer(Artefact::Spelleater_Pendant),
            to_integer(Artefact::Scroll_of_Dark_Unravelling),
            to_integer(Artefact::Spell_Familiar)
    };

    constexpr std::array<int, 7> g_despoilerArtefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Crown_of_Hellish_Adoration),
            to_integer(Artefact::Helm_of_Many_Eyes),
            to_integer(Artefact::Armour_of_Tortured_Souls),
            to_integer(Artefact::Diabolic_Mantle),
            to_integer(Artefact::Doombringer_Blade),
            to_integer(Artefact::Realmwarpers_Twist_Rune)
    };

} // SlavesToDarkness

#endif //SLAVESTODARKNESSPRIVATE_H