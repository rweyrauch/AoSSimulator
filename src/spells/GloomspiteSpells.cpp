/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/GloomspiteSpells.h>

#include "spells/GloomspiteSpells.h"

std::string ToString(LoreOfTheMoonclans which)
{
    switch (which)
    {
        case LoreOfTheMoonclans::None:
            return "None";
        case LoreOfTheMoonclans::VindictiveGlare:
            return "Vindictive Glare";
        case LoreOfTheMoonclans::ItchyNuisance:
            return "Itchy Nuisance";
        case LoreOfTheMoonclans::TheGreatGreenSpite:
            return "The Great Green Spite";
        case LoreOfTheMoonclans::TheHandOfGork:
            return "The Hand of Gork";
        case LoreOfTheMoonclans::SquigLure:
            return "Squig Lure";
        case LoreOfTheMoonclans::CallDaMoon:
            return "Call da Moon";
    }
    return "";
}

bool FromString(const std::string &enumString, LoreOfTheMoonclans &outLore)
{
    bool valid = true;

    if (enumString == "None")
        outLore = LoreOfTheMoonclans::None;
    else if (enumString == "Vindictive Glare")
        outLore = LoreOfTheMoonclans::VindictiveGlare;
    else if (enumString == "Itchy Nuisance")
        outLore = LoreOfTheMoonclans::ItchyNuisance;
    else if (enumString == "The Great Green Spite")
        outLore = LoreOfTheMoonclans::TheGreatGreenSpite;
    else if (enumString == "The Hand of Gork")
        outLore = LoreOfTheMoonclans::TheHandOfGork;
    else if (enumString == "Squig Lure")
        outLore = LoreOfTheMoonclans::SquigLure;
    else if (enumString == "Call da Moon")
        outLore = LoreOfTheMoonclans::CallDaMoon;
    return valid;
}

std::string ToString(LoreOfTheSpiderFangs which)
{
    switch (which)
    {
        case LoreOfTheSpiderFangs::None:
            return "None";
        case LoreOfTheSpiderFangs::DeadlyWebbing:
            return "Deadly Webbing";
        case LoreOfTheSpiderFangs::VenomousSpiderlings:
            return "Venomous Spiderlings";
        case LoreOfTheSpiderFangs::ScuttlingTerrors:
            return "Scuttling Terrors";
        case LoreOfTheSpiderFangs::SneakyDistraction:
            return "Sneaky Distraction";
        case LoreOfTheSpiderFangs::CurseOfDaSpiderGod:
            return "Curse of da Spider God";
        case LoreOfTheSpiderFangs::GiftOfDaSpiderGod:
            return "Gift of da Spider God";
    }
    return "";
}

bool FromString(const std::string &enumString, LoreOfTheSpiderFangs &outLore)
{
    bool valid = true;

    if (enumString == "None")
        outLore = LoreOfTheSpiderFangs::None;
    else if (enumString == "Deadly Webbing")
        outLore = LoreOfTheSpiderFangs::DeadlyWebbing;
    else if (enumString == "Venomous Spiderlings")
        outLore = LoreOfTheSpiderFangs::VenomousSpiderlings;
    else if (enumString == "Scuttling Terrors")
        outLore = LoreOfTheSpiderFangs::ScuttlingTerrors;
    else if (enumString == "Sneaky Distraction")
        outLore = LoreOfTheSpiderFangs::SneakyDistraction;
    else if (enumString == "Curse of da Spider God")
        outLore = LoreOfTheSpiderFangs::CurseOfDaSpiderGod;
    else if (enumString == "Gift of da Spider God")
        outLore = LoreOfTheSpiderFangs::GiftOfDaSpiderGod;
    return valid;
}

DamageSpell *CreateVindictiveGlare(Unit *caster)
{
    return new DamageSpell(caster, "Vindictive Glare", 5, 12, RAND_D3);
}

Spell *CreateLoreOfTheSpiderFangs(LoreOfTheSpiderFangs which, Unit *caster)
{
    return nullptr;
}

Spell *CreateLoreOfTheMoonclans(LoreOfTheMoonclans which, Unit *caster)
{
    return nullptr;
}
