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

    enum class Lore : int {
        None,

        // LoreOfTheMoonclans
        Vindictive_Glare,
        Itchy_Nuisance,
        The_Great_Green_Spite,
        The_Hand_Of_Gork,
        Squig_Lure,
        Call_Da_Moon,

        // LoreOfTheSpiderFangs
        Deadly_Webbing,
        Venomous_Spiderlings,
        Scuttling_Terrors,
        Sneaky_Distraction,
        Curse_Of_Da_Spider_God,
        Gift_Of_Da_Spider_God
    };

    std::string ToString(Lore which);

    bool FromString(const std::string &enumString, Lore &outLore);

    Spell *CreateLore(Lore which, Unit *caster);

    DamageSpell *CreateVindictiveGlare(Unit *caster);

} // namespace GloomspiteGitz

#endif //GLOOMSPITESPELLS_H