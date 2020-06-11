/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOREOFTHESTORM_H
#define LOREOFTHESTORM_H

#include <Spell.h>

namespace StormcastEternals {

    enum class Lore : int {

        // LoreOfTheStorm
        Lightning_Blast,
        Starfall,
        Thundershock,
        Azyrite_Halo,
        Chain_Lightning,
        Stormcaller,

        // LoreOfInvigoration
        Terrifying_Aspect,
        Celestial_Blades,
        Speed_of_Lightning
    };

    std::string ToString(Lore which);

    bool FromString(const std::string &enumString, Lore &outLore);

    DamageSpell *CreateLightningBlast(Unit *caster);

    AreaOfEffectSpell *CreateStarfall(Unit *caster);

    AreaOfEffectSpell *CreateThundershock(Unit *caster);

    Spell *CreateAzyriteHalo(Unit *caster);

    AreaOfEffectSpell *CreateChainLightning(Unit *caster);

    AreaOfEffectSpell *CreateStormcaller(Unit *caster);

    Spell *CreateTerrifyingAspect(Unit *caster);

    Spell *CreateCelestialBlades(Unit *caster);

    Spell *CreateSpeedOfLightning(Unit *caster);

    Spell *CreateLore(Lore which, Unit *caster);

} // namespace StormcastEternals

#endif //LOREOFTHESTORM_H