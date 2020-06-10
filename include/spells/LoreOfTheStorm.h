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
#include <array>

namespace StormcastEternals {

    enum Lore : int {

        // LoreOfTheStorm
        LightningBlast,
        Starfall,
        Thundershock,
        AzyriteHalo,
        ChainLightning,
        Stormcaller,

        // LoreOfInvigoration
        TerrifyingAspect,
        CelestialBlades,
        SpeedOfLightning
    };

    const std::array<int, 6> g_loreOfTheStorm = {
        LightningBlast,
        Starfall,
        Thundershock,
        AzyriteHalo,
        ChainLightning,
        Stormcaller,
    };

    const std::array<int, 3> g_loreOfInvigoration = {
        TerrifyingAspect,
        CelestialBlades,
        SpeedOfLightning
    };

    const std::array<int, 9> g_lore = {
        LightningBlast,
        Starfall,
        Thundershock,
        AzyriteHalo,
        ChainLightning,
        Stormcaller,
        TerrifyingAspect,
        CelestialBlades,
        SpeedOfLightning
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