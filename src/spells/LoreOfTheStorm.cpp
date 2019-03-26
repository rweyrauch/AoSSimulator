/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cfloat>
#include <spells/LoreOfTheStorm.h>

DamageSpell *CreateLightningBlast(Unit *caster)
{
    return new DamageSpell(caster, "Lightning Blast", 5, FLT_MAX, RAND_D3);
}

AreaOfEffectSpell *CreateStarfall(Unit *caster)
{
    return new AreaOfEffectSpell(caster, "Starfall", 5, 12.0f, 3.0f, 1, 4);
}

AreaOfEffectSpell *CreateThundershock(Unit *caster)
{
    return new AreaOfEffectSpell(caster, "Thundershock", 6, 0.0f, 6.0f, 1, 4);
}

AreaOfEffectSpell *CreateStormcaller(Unit *caster)
{
    return new AreaOfEffectSpell(caster, "Stormcaller", 7, 0.0f, FLT_MAX, RAND_D3, 6);
}

Spell *CreateAzyriteHalo(Unit *caster)
{
    return nullptr;
}

AreaOfEffectSpell *CreateChainLightning(Unit *caster)
{
    return nullptr;
}

Spell *CreateTerrifyingAspect(Unit *caster)
{
    return nullptr;
}

Spell *CreateCelestialBlades(Unit *caster)
{
    return nullptr;
}

Spell *CreateSpeedOfLightning(Unit *caster)
{
    return nullptr;
}
