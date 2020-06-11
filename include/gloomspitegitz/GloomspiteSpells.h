/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GLOOMSPITESPELLS_H
#define GLOOMSPITESPELLS_H

#include <Spell.h>
#include <array>

namespace GloomspiteGitz {

    enum Lore {
        None,

        // LoreOfTheMoonclans
        VindictiveGlare,
        ItchyNuisance,
        TheGreatGreenSpite,
        TheHandOfGork,
        SquigLure,
        CallDaMoon,

        // LoreOfTheSpiderFangs
        DeadlyWebbing,
        VenomousSpiderlings,
        ScuttlingTerrors,
        SneakyDistraction,
        CurseOfDaSpiderGod,
        GiftOfDaSpiderGod
    };

    const std::array<int, 7> g_loreOfTheMoonclans = {
        None,
        VindictiveGlare,
        ItchyNuisance,
        TheGreatGreenSpite,
        TheHandOfGork,
        SquigLure,
        CallDaMoon
    };

    const std::array<int, 7> g_loreOfTheSpiderFangs = {
        None,
        DeadlyWebbing,
        VenomousSpiderlings,
        ScuttlingTerrors,
        SneakyDistraction,
        CurseOfDaSpiderGod,
        GiftOfDaSpiderGod
    };


    std::string ToString(Lore which);

    bool FromString(const std::string &enumString, Lore &outLore);

    Spell *CreateLore(Lore which, Unit *caster);

    DamageSpell *CreateVindictiveGlare(Unit *caster);

} // namespace GloomspiteGitz

#endif //GLOOMSPITESPELLS_H