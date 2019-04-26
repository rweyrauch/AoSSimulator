/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cfloat>
#include <spells/LoreOfTheStorm.h>

std::string ToString(LoreOfTheStorm which)
{
    switch (which)
    {
        case LoreOfTheStorm::None:
            return "None";
        case LoreOfTheStorm::LightningBlast:
            return "Lightning Blast";
        case LoreOfTheStorm::Starfall:
            return "Starfall";
        case LoreOfTheStorm::Thundershock:
            return "Thundershock";
        case LoreOfTheStorm::AzyriteHalo:
            return "Azyrite Halo";
        case LoreOfTheStorm::ChainLightning:
            return "Chain Lightning";
        case LoreOfTheStorm::Stormcaller:
            return "Stormcaller";
    }
    return "";
}

bool FromString(const std::string &enumString, LoreOfTheStorm &outLore)
{
    bool valid = true;

    if (enumString == "None")
        outLore =LoreOfTheStorm::None;
    else if (enumString == "Lightning Blast")
        outLore = LoreOfTheStorm::LightningBlast;
    else if (enumString == "Starfall")
        outLore = LoreOfTheStorm::Starfall;
    else if (enumString == "Thundershock")
        outLore = LoreOfTheStorm::Thundershock;
    else if (enumString == "Azyrite Halo")
        outLore = LoreOfTheStorm::AzyriteHalo;
    else if (enumString == "Chain Lightning")
        outLore = LoreOfTheStorm::ChainLightning;
    else if (enumString == "Stormcaller")
        outLore = LoreOfTheStorm::Stormcaller;
    else
        valid = false;
    return valid;
}

std::string ToString(LoreOfInvigoration which)
{
    switch (which)
    {
        case LoreOfInvigoration::None:
            return "None";
        case LoreOfInvigoration::TerrifyingAspect:
            return "Terrifying Aspect";
        case LoreOfInvigoration::CelestialBlades:
            return "Celestial Blades";
        case LoreOfInvigoration::SpeedOfLightning:
            return "SpeedOfLightning";
    }
    return "";
}

bool FromString(const std::string &enumString, LoreOfInvigoration &outLore)
{
    bool valid = true;

    if (enumString == "None")
        outLore = LoreOfInvigoration::None;
    else if (enumString == "Terrifying Aspect")
        outLore = LoreOfInvigoration::TerrifyingAspect;
    else if (enumString == "Celestial Blades")
        outLore = LoreOfInvigoration::CelestialBlades;
    else if (enumString == "Speed of Lightning")
        outLore = LoreOfInvigoration::SpeedOfLightning;
    else
        valid = false;
    return valid;
}

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
    return new BuffModifierSpell(caster, "Celestial Blades", 5, 18.0f, ToWoundMelee, 1, true);
}

Spell *CreateSpeedOfLightning(Unit *caster)
{
    return new BuffRerollSpell(caster, "Speed of Lightning", 5, 9.0f, ChargeDistance, RerollFailed, true);
}

Spell *CreateLoreOfTheStorm(LoreOfTheStorm which, Unit *caster)
{
    switch (which)
    {
        case LoreOfTheStorm::LightningBlast:
            return CreateLightningBlast(caster);
        case LoreOfTheStorm::Starfall:
            return CreateStarfall(caster);
        case LoreOfTheStorm::Thundershock:
            return CreateThundershock(caster);
        case LoreOfTheStorm::AzyriteHalo:
            return CreateAzyriteHalo(caster);
        case LoreOfTheStorm::ChainLightning:
            return CreateChainLightning(caster);
        case LoreOfTheStorm::Stormcaller:
            return CreateStormcaller(caster);
        default:
            return nullptr;
    }
}

Spell *CreateLoreOfInvigoration(LoreOfInvigoration which, Unit *caster)
{
    switch (which)
    {
        case LoreOfInvigoration::CelestialBlades:
            return CreateCelestialBlades(caster);
        case LoreOfInvigoration::TerrifyingAspect:
            return CreateTerrifyingAspect(caster);
        case LoreOfInvigoration::SpeedOfLightning:
            return CreateSpeedOfLightning(caster);
        case LoreOfInvigoration::None:
            return nullptr;
    }
    return nullptr;
}
