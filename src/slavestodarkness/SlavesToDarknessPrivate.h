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
            ToInteger(DamnedLegion::Ravagers),
            ToInteger(DamnedLegion::Cabalists),
            ToInteger(DamnedLegion::Despoilers),
            ToInteger(DamnedLegion::Host_Of_The_Everchosen)
    };

    constexpr std::array<int, 5> g_markOfChaos = {
            ToInteger(MarkOfChaos::Undivided),
            ToInteger(MarkOfChaos::Nurgle),
            ToInteger(MarkOfChaos::Khorne),
            ToInteger(MarkOfChaos::Slaanesh),
            ToInteger(MarkOfChaos::Tzeentch)
    };

    constexpr std::array<int, 7> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Binding_Damnation),
            ToInteger(Lore::Spite_Tongue_Curse),
            ToInteger(Lore::Whispers_Of_Chaos),
            ToInteger(Lore::Mask_Of_Darkness),
            ToInteger(Lore::Call_To_Glory),
            ToInteger(Lore::Ruinous_Vigour)
    };

    constexpr std::array<int, 7> g_ravagersCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Bolstered_By_Hate),
            ToInteger(CommandTrait::Unquestioned_Resolve),
            ToInteger(CommandTrait::Favoured_Of_The_Pantheon),
            ToInteger(CommandTrait::Eternal_Vendetta),
            ToInteger(CommandTrait::Flames_Of_Spite),
            ToInteger(CommandTrait::Master_Of_Deception)
    };

    constexpr std::array<int, 7> g_cabalistCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Bolstered_By_Hate),
            ToInteger(CommandTrait::Lord_Of_Terror),
            ToInteger(CommandTrait::Favoured_Of_The_Pantheon),
            ToInteger(CommandTrait::Mighty_Ritualist),
            ToInteger(CommandTrait::Blasphemous_Influence),
            ToInteger(CommandTrait::All_For_One)
    };

    constexpr std::array<int, 7> g_despoilerCommandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Bolstered_By_Hate),
            ToInteger(CommandTrait::Lord_Of_Terror),
            ToInteger(CommandTrait::Lightning_Reflexes),
            ToInteger(CommandTrait::Radiance_Of_Dark_Glory),
            ToInteger(CommandTrait::Distorting_Miasma),
            ToInteger(CommandTrait::Paragon_Of_Ruin)
    };

    constexpr std::array<int, 7> g_ravagerArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Hellfire_Sword),
            ToInteger(Artefact::Blasphemous_Cuirass),
            ToInteger(Artefact::Helm_Of_The_Oppressor),
            ToInteger(Artefact::Cloak_Of_The_Relentless_Conqueror),
            ToInteger(Artefact::Mark_Of_The_High_Favoured),
            ToInteger(Artefact::Desecrator_Gauntlets)
    };

    constexpr std::array<int, 7> g_cabalistArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Soul_Feeder),
            ToInteger(Artefact::Black_Athame),
            ToInteger(Artefact::Infernal_Puppet),
            ToInteger(Artefact::Spelleater_Pendant),
            ToInteger(Artefact::Scroll_Of_Dark_Unravelling),
            ToInteger(Artefact::Spell_Familiar)
    };

    constexpr std::array<int, 7> g_despoilerArtefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Crown_Of_Hellish_Adoration),
            ToInteger(Artefact::Helm_Of_Many_Eyes),
            ToInteger(Artefact::Armour_Of_Tortured_Souls),
            ToInteger(Artefact::Diabolic_Mantle),
            ToInteger(Artefact::Doombringer_Blade),
            ToInteger(Artefact::Realmwarpers_Twist_Rune)
    };

} // SlavesToDarkness

#endif //SLAVESTODARKNESSPRIVATE_H