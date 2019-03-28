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

enum class LoreOfTheStorm : int
{
    None = 0,
    LightningBlast,
    Starfall,
    Thundershock,
    AzyriteHalo,
    ChainLightning,
    Stormcaller,
};

std::string ToString(LoreOfTheStorm which);
bool FromString(const std::string &enumString, LoreOfTheStorm &outLore);

enum class LoreOfInvigoration : int
{
    None = 0,
    TerrifyingAspect,
    CelestialBlades,
    SpeedOfLightning,
};

std::string ToString(LoreOfInvigoration which);
bool FromString(const std::string &enumString, LoreOfInvigoration &outLore);

DamageSpell* CreateLightningBlast(Unit* caster);
AreaOfEffectSpell* CreateStarfall(Unit* caster);
AreaOfEffectSpell* CreateThundershock(Unit* caster);
Spell* CreateAzyriteHalo(Unit* caster);
AreaOfEffectSpell* CreateChainLightning(Unit* caster);
AreaOfEffectSpell* CreateStormcaller(Unit* caster);

Spell* CreateLoreOfTheStorm(LoreOfTheStorm which, Unit* caster);


Spell* CreateTerrifyingAspect(Unit* caster);
Spell* CreateCelestialBlades(Unit* caster);
Spell* CreateSpeedOfLightning(Unit* caster);

Spell* CreateLoreOfInvigoration(LoreOfInvigoration which, Unit* caster);

#endif //LOREOFTHESTORM_H